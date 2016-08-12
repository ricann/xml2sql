#ifndef __X2SPARSE_H__
#define __X2SPARSE_H__

#include <libxml/parser.h>
#include <libxml/tree.h>

#define PARSE_SUCCESS		0
#define PARSE_FAIL			1

#define XML_MAXSIZE			1024
#define XML_MAX_DEPTH		32
#define XML_MIN_BUF			128
#define XML_MAX_BUF			4096

#define XML_ROOT		"config"
#define XML_ARRAY		"array"
#define XML_KEY			"key"

#define XML_NAME		"sys_config.xml"
#define XML_ENCODE		"UTF-8"

int xml_open(const char *);
int xml_parse();
int xml_parse_keycat();
void xml_traverse(xmlNodePtr);
void xml_traverse_keycat(xmlNodePtr);
int xml_get_key(const char *key, char *value, int len);
int xml_set_key(const char *key, char *value, int len);
void xml_close();
void xml_print();

int xml_save2db();

#endif /* __X2SPARSE_H__ */
