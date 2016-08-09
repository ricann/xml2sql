#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

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

	static int depth = -1;
	static char catbuf[XML_MAX_BUF];
	static char arrbuf[XML_MAX_DEPTH][XML_MIN_BUF];

	if(!nodePtr || depth>=XML_MAX_DEPTH)
		return 1;

	if(++depth > 0) {
		len1 = strlen(arrbuf[depth-1]) + 1;
		strcat(catbuf, arrbuf[depth-1]);
		strcat(catbuf, ".");
	}

	curNode = nodePtr->xmlChildrenNode;
	while(curNode) {
		if(strcmp((char *)curNode->name, "text") == 0){
			curNode = curNode->next;	
			continue;
		}
		
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

		if(curNode->xmlChildrenNode)
			xml_traverse(curNode);
		curNode = curNode->next;
	}

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

