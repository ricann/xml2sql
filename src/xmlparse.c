#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "xmlparse.h"

int count = 0;
xmlChar *nodeName[XML_MAXSIZE];
xmlChar *nodeProp[XML_MAXSIZE];
xmlChar *nodeValue[XML_MAXSIZE];

xmlDocPtr doc; 
xmlNodePtr rootNode;

int xml_open(char *fname);
int xml_traverse(xmlNodePtr nodePtr);
void xml_close();
void xml_print();

int main()
{
	xml_open(XML_NAME);
	xml_traverse(rootNode);
	xml_print();
	xml_close();

	return 0;
}

int xml_open(char *fname)
{
	// open xml file
	doc = xmlReadFile(XML_NAME, XML_ENCODE, XML_PARSE_RECOVER);
	if(!doc) {
		printf("open xml error!\n");
		exit(1);
	}

	// get root element
	rootNode = xmlDocGetRootElement(doc);
	if(!rootNode){
		printf("the document is empty!\n");
		xmlFreeDoc(doc);
		exit(1);
	}

	return 0;
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
		
		nodeName[count] = (xmlChar *)curNode->name;
		nodeProp[count] = xmlGetProp(curNode, (const xmlChar *)"name");
		if(curNode->xmlChildrenNode->next == NULL)
			nodeValue[count] = xmlNodeGetContent(curNode);
		else
			nodeValue[count] = (xmlChar *)"";
		count++;

		if(curNode->xmlChildrenNode)
			xml_traverse(curNode);
		curNode = curNode->next;
	}

	return 0;
}

void xml_close()
{
	xmlFreeDoc(doc);
}

void xml_print()
{
	int i;

	printf("%-16s%-32s%-32s\n", "name", "prop", "value");
	printf("----------------------------------------------------\n");
	for(i=0; i<count; i++) 
		printf("%-16s%-32s%-32s\n", nodeName[i], nodeProp[i], nodeValue[i]);
	
}

