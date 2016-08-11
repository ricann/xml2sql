#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "sqlapi.h"
#include "xmlparse.h"

typedef struct xmlinfo_s{
	int count;

	xmlChar *nodeName[XML_MAXSIZE];
	xmlChar *nodeProp[XML_MAXSIZE];
	xmlChar *nodeValue[XML_MAXSIZE];
	
	xmlDocPtr doc; 
	xmlNodePtr rootNode;
}xmlinfo_t;

static xmlinfo_t xinfo;

int xml_open(char *fname)
{
	// init xinfo struct
	memset(&xinfo, 0, sizeof(xinfo));

	// open xml file
	xinfo.doc = xmlReadFile(XML_NAME, XML_ENCODE, XML_PARSE_RECOVER);
	if(!xinfo.doc) {
		printf("open xml error!\n");
		exit(1);
	}

	// get root element
	xinfo.rootNode = xmlDocGetRootElement(xinfo.doc);
	if(!xinfo.rootNode){
		printf("the document is empty!\n");
		xmlFreeDoc(xinfo.doc);
		exit(1);
	}

	return 0;
}

int xml_parse()
{
	return xml_traverse(xinfo.rootNode);
}

int xml_traverse(xmlNodePtr nodePtr)
{
	int len1 = 0;
	int len2 = 0;
	xmlNodePtr curNode = NULL;

	// used to save every layer's array name info
	static int depth = -1;
	static char catbuf[XML_MAX_BUF];
	static char arrbuf[XML_MAX_DEPTH][XML_MIN_BUF];

	if(!nodePtr || depth>=XML_MAX_DEPTH)
		return 1;

	// strcat every layer's array info
	if(++depth > 0) {
		len1 = strlen(arrbuf[depth-1]) + 1;
		strcat(catbuf, arrbuf[depth-1]);
		strcat(catbuf, ".");
	}

	curNode = nodePtr->xmlChildrenNode;
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

		if(curNode->xmlChildrenNode->next == NULL)
			xinfo.nodeValue[xinfo.count] = xmlNodeGetContent(curNode);
		else
			xinfo.nodeValue[xinfo.count] = (xmlChar *)"";
		xinfo.count++;
		if(xinfo.count == XML_MAXSIZE) {
			printf("buffer is full, exit...\n");
			exit(1);
		}

		// traverse next xml layer
		if(curNode->xmlChildrenNode)
			xml_traverse(curNode);
		curNode = curNode->next;
	}

	// change str1.str2.str3 to str1.str2
	if(depth-- > 0) {
		catbuf[strlen(catbuf) - len1] = '\0';
	}

	return 0;
}

void xml_close()
{
	xmlFreeDoc(xinfo.doc);
}

void xml_print()
{
	int i;

	printf("%-16s%-64s%-32s\n", "name", "prop", "value");
	printf("-----------------------------------------------------------------------------------------\n");
	for(i=0; i<xinfo.count; i++) 
		printf("%-16s%-64s%-32s\n", xinfo.nodeName[i], xinfo.nodeProp[i], xinfo.nodeValue[i]);
	
}

int xml_save2db()
{
	int i;
	int id;
	char sqlbuf[XML_MAX_BUF];

	sql_open(DEFAULT_DB_NAME);	
	sql_create_table(CREATE_TABLE_STR);
	for(i=0, id=0; i<xinfo.count; i++) {
		if(strcmp((char *)xinfo.nodeName[i], "key") != 0)
			continue;
		snprintf(sqlbuf, XML_MAX_BUF, INSERT_FMT_STR, 
			id++, (char *)xinfo.nodeProp[i], (char *)xinfo.nodeValue[i], 2);
		sql_insert(sqlbuf);
	}

	sql_close();
	return 0;
}
