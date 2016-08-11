#ifndef __XMLPARSE_H__
#define __XMLPARSE_H__

#include <libxml/parser.h>
#include <libxml/tree.h>

#define XML_MAXSIZE			1024
#define XML_MAX_DEPTH		32
#define XML_MIN_BUF			128
#define XML_MAX_BUF			4096

#define XML_ROOT		"config"
#define XML_ARRAY		"array"
#define XML_KEY			"key"

#define XML_NAME		"sys_config.xml"
#define XML_ENCODE		"UTF-8"

int xml_open(char *fname);
int xml_parse();
int xml_traverse(xmlNodePtr);
void xml_close();
void xml_print();

int xml_save2db();

#endif /* __XMLPARSE_H__ */
