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
	xmlNodePtr curNode = NULL;

	if(!nodePtr)
		return 1;

	curNode = nodePtr->xmlChildrenNode;
	while(curNode) {
		if(strcmp((char *)curNode->name, "text") == 0){
			curNode = curNode->next;	
			continue;
		}
		
		xinfo.nodeName[xinfo.count] = (xmlChar *)curNode->name;
		xinfo.nodeProp[xinfo.count] = xmlGetProp(curNode, (const xmlChar *)"name");
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

	return 0;
}

void xml_close()
{
	xmlFreeDoc(xinfo.doc);
}

void xml_print()
{
	int i;

	printf("%-16s%-32s%-32s\n", "name", "prop", "value");
	printf("----------------------------------------------------\n");
	for(i=0; i<xinfo.count; i++) 
		printf("%-16s%-32s%-32s\n", xinfo.nodeName[i], xinfo.nodeProp[i], xinfo.nodeValue[i]);
	
}

