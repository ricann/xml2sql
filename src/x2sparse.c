#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "x2sdb.h"
#include "x2sutil.h"
#include "x2sconf.h"
#include "x2sparse.h"

typedef struct xmlinfo_s{
	int count;

	xmlChar *nodeName[XML_MAXSIZE];
	xmlChar *nodeProp[XML_MAXSIZE];
	xmlChar *nodeValue[XML_MAXSIZE];
	
	xmlDocPtr doc; 
	xmlNodePtr rootNode;
}xmlinfo_t;

static xmlinfo_t xinfo;

/******************************************************************************
 * *Function: xml_open
 * *Description: open xml file
 * *Input: fname(file name)
 * *Output: none
 * *Return: PARSE_SUCCESS/PARSE_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int xml_open(const char *fname)
{
	// init xinfo struct
	memset(&xinfo, 0, sizeof(xinfo));

	// open xml file
	xinfo.doc = xmlReadFile(fname, XML_ENCODE, XML_PARSE_RECOVER);
	if(!xinfo.doc) {
		x2s_dbg("open xml error!\n");
		return PARSE_FAIL;
	}

	// get root element
	xinfo.rootNode = xmlDocGetRootElement(xinfo.doc);
	if(!xinfo.rootNode){
		x2s_dbg("the document is empty!\n");
		xmlFreeDoc(xinfo.doc);
		return PARSE_FAIL;
	}

	return PARSE_SUCCESS;
}

/******************************************************************************
 * *Function: xml_parse
 * *Description: parse a xml file, save parse result into xinfo var 
 * *Input: none
 * *Output: none
 * *Return: PARSE_SUCCESS/PARSE_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int xml_parse()
{
	xml_traverse(xinfo.rootNode);

	return PARSE_SUCCESS;
}

/******************************************************************************
 * *Function: xml_traverse
 * *Description: xml_parse func's implementation
 * *Input: curNode(xml files's root node)
 * *Output: none
 * *Return: none
 * *Date: 2016/8/22
 * ****************************************************************************/
void xml_traverse(xmlNodePtr curNode)
{
	if(!curNode)
		return;

	while(curNode) {
		// not handle text name
		if(strcmp((char *)curNode->name, "text") == 0){
			curNode = curNode->next;	
			continue;
		}
		
		// save array name, key name, key value
		xinfo.nodeName[xinfo.count] = (xmlChar *)curNode->name;
		xinfo.nodeProp[xinfo.count] = xmlGetProp(curNode, (const xmlChar *)"name");

		if(curNode->xmlChildrenNode && curNode->xmlChildrenNode->next == NULL)
			xinfo.nodeValue[xinfo.count] = xmlNodeGetContent(curNode);
		else 
			xinfo.nodeValue[xinfo.count] = (xmlChar *)"";
		
		xinfo.count++;
		if(xinfo.count == XML_MAXSIZE) {
			x2s_dbg("buffer is full, exit...\n"); 
			exit(1);
		}

		// traverse next xml layer
		xml_traverse(curNode->xmlChildrenNode);
		curNode = curNode->next;
	}
}

/******************************************************************************
 * *Function: xml_parse_keycat
 * *Description: parse a xml file, save parse result into xinfo var, and 
 * * *nodeProp save a catenated key name 
 * *Input: none
 * *Output: none
 * *Return: PARSE_SUCCESS/PARSE_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int xml_parse_keycat()
{
	xml_traverse_keycat(xinfo.rootNode);

	return PARSE_SUCCESS;
}

/******************************************************************************
 * *Function: xml_traverse_keycat
 * *Description: xml_parse_keycat func's implementation
 * *Input: curNode(xml files's root node)
 * *Output: none
 * *Return: none
 * *Date: 2016/8/22
 * ****************************************************************************/
void xml_traverse_keycat(xmlNodePtr curNode)
{
	int len1 = 0;
	int len2 = 0;

	// used to save every layer's array name info
	static int depth = -1;
	static char catbuf[XML_MAX_BUF];
	static char arrbuf[XML_MAX_DEPTH][XML_MIN_BUF];

	if(!curNode || depth>=XML_MAX_DEPTH)
		return;

	// strcat every layer's array info
	if(++depth > 0) {
		len1 = strlen(arrbuf[depth-1]) + 1;
		strcat(catbuf, arrbuf[depth-1]);
		strcat(catbuf, ".");
	}

	while(curNode) {
		// not handle text name
		if(strcmp((char *)curNode->name, "text") == 0){
			curNode = curNode->next;	
			continue;
		}
		
		// save array name, key name, key value
		xinfo.nodeName[xinfo.count] = (xmlChar *)curNode->name;
		xinfo.nodeProp[xinfo.count] = xmlGetProp(curNode, (const xmlChar *)"name");
		if(strcmp((char *)curNode->name, "array") == 0)
			strcpy(arrbuf[depth], (char *)xinfo.nodeProp[xinfo.count]);
		else if(strcmp((char *)curNode->name, "key") == 0){
			len2 = strlen(((char *)xinfo.nodeProp[xinfo.count]));
			strcat(catbuf, (char *)xinfo.nodeProp[xinfo.count]);
			xmlSetProp(curNode, (const xmlChar *)"name", (const xmlChar *)catbuf);
			xinfo.nodeProp[xinfo.count] = xmlGetProp(curNode, (const xmlChar *)"name");
			catbuf[strlen(catbuf) - len2] = '\0';
		}

		if(curNode->xmlChildrenNode && curNode->xmlChildrenNode->next == NULL)
			xinfo.nodeValue[xinfo.count] = xmlNodeGetContent(curNode);
		else
			xinfo.nodeValue[xinfo.count] = (xmlChar *)"";
		xinfo.count++;
		if(xinfo.count == XML_MAXSIZE) {
			x2s_dbg("buffer is full, exit...\n");
			exit(1);
		}

		// traverse next xml layer
		xml_traverse(curNode->xmlChildrenNode);
		curNode = curNode->next;
	}

	// change str1.str2.str3 to str1.str2
	if(depth-- > 0) 
		catbuf[strlen(catbuf) - len1] = '\0';
}

/******************************************************************************
 * *Function: xml_get_value
 * *Description: get key's value
 * *Input: key(primary key name), len(value's buffer length)
 * *Output: value(key's value)
 * *Return: PARSE_SUCCESS/PARSE_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int xml_get_value(const char *key, char *value, int len)
{
	int i;
	
	for(i=0; i<xinfo.count; i++){
		if(xinfo.nodeProp[i] == NULL)
			continue;

		if(strcmp(key, (char *)xinfo.nodeProp[i]) == 0) {
			strncpy(value, (char *)xinfo.nodeValue[i], len);	
			return PARSE_SUCCESS;
		}
	} 

	return PARSE_FAIL;
}

/******************************************************************************
 * *Function: xml_get_multi_value
 * *Description: get key's multi-value, used to get configured xml file names
 * *Input: key(primary key name)
 * *Output: value(key's value), nxml(value's number)
 * *Return: PARSE_SUCCESS/PARSE_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int xml_get_multi_value(const char *key, char value[CONF_XML_MAX][CONF_NAME_LEN], int *nxml)
{
	int i;

	if(!nxml)
		return PARSE_FAIL;
	*nxml = 0;
	
	for(i=0; i<xinfo.count; i++){
		if(*nxml >= CONF_XML_MAX)
			return PARSE_SUCCESS;

		if(xinfo.nodeProp[i] == NULL)
			continue;

		if(strcmp(key, (char *)xinfo.nodeProp[i]) == 0) {
			strncpy(value[*nxml], (char *)xinfo.nodeValue[i], CONF_NAME_LEN);	
			(*nxml)++;
		}
	} 

	if(*nxml > 0)
		return PARSE_SUCCESS;
	return PARSE_FAIL;
}

/******************************************************************************
 * *Function: xml_close
 * *Description: close xml file
 * *Input: none
 * *Output: none
 * *Return: none
 * *Date: 2016/8/22
 * ****************************************************************************/
void xml_close()
{
	xmlFreeDoc(xinfo.doc);
}

/******************************************************************************
 * *Function: xml_print
 * *Description: print xml files name/prop/value info
 * *Input: none
 * *Output: none
 * *Return: none
 * *Date: 2016/8/22
 * ****************************************************************************/
void xml_print()
{
	int i;

	x2s_dbg("%-16s%-64s%-32s\n", "name", "prop", "value");
	x2s_dbg("-----------------------------------------------------------------------------------------\n");
	for(i=0; i<xinfo.count; i++) 
		x2s_dbg("%-16s%-64s%-32s\n", xinfo.nodeName[i], xinfo.nodeProp[i], xinfo.nodeValue[i]);
	
}

/******************************************************************************
 * *Function: xml_save2db
 * *Description: save xml info into a db file
 * *Input: none
 * *Output: none
 * *Return: PARSE_SUCCESS/PARSE_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int xml_save2db()
{
	int i;
	int id;

	if(db_open() == DB_FAIL) {
		x2s_dbg("db_open fail\n");
		return PARSE_FAIL;
	}	

	for(i=0, id=0; i<xinfo.count; i++) {
		if(strcmp((char *)xinfo.nodeName[i], "key") != 0)
			continue;
		if(db_insert_item(id, (char *)xinfo.nodeProp[i], (char *)xinfo.nodeValue[i], 2) == DB_FAIL) {
			x2s_dbg("db_insert_item fail\n");
			return PARSE_FAIL;
		}	
	}

	db_close();
	return PARSE_SUCCESS;
}
