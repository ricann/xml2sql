#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "x2sparse.h"
#include "x2sutil.h"
#include "x2sconf.h"

#define CONF_FNAME					"x2s_config.xml"
#define	CONF_FMODE					(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	KEYNAME_DBNAME				"dbname" 
#define	KEYNAME_DBTABLE				"dbtable" 
#define	KEYNAME_DEBUG				"debug" 
#define	KEYNAME_DEBUG_FNAME			"fname" 
#define	KEYNAME_XML_FNAME			"xmlname" 

#define	KEYVALUE_DFT_DBNAME			"x2s.db" 
#define	KEYVALUE_DFT_DBTABLE		"X2STABLE" 
#define	KEYVALUE_DFT_DEBUG			(DEBUG_STDOUT)
#define	KEYVALUE_DFT_DEBUG_FNAME	"x2s.dbg" 
#define	KEYVALUE_DFT_XML_FNAME		"sys_config.xml" 

glob_conf_t gconf;

static void conf_default();
static int conf_init();
static int conf_get_dbname();
static int conf_get_dbtable();
static int conf_get_debug();
static int conf_get_debug_fname();
static int conf_get_xml_fname();
static int conf_get_debug_fd();
static void conf_finish();
static void conf_print();

/******************************************************************************
 * *Function: conf_read
 * *Description: read config file and save its contents
 * *Input: none
 * *Output: none
 * *Return: none
 * *Date: 2016/8/22
 * ****************************************************************************/
void conf_read()
{
	// open xml file and read its contents
	if(conf_init() == CONF_FAIL) {
		conf_default();
		return;
	}

	conf_get_dbname();
	conf_get_dbtable();
	conf_get_debug();
	conf_get_debug_fname();
	conf_get_debug_fd();
	conf_get_xml_fname();

	// close xml file
	conf_finish();

	// for debug
	conf_print();

	return;
}

/******************************************************************************
 * *Function: conf_default
 * *Description: set gconf var to default value
 * *Input: none
 * *Output: none
 * *Return: none
 * *Date: 2016/8/22
 * ****************************************************************************/
void conf_default()
{
	strncpy(gconf.dbname, KEYVALUE_DFT_DBNAME, CONF_NAME_LEN);
	strncpy(gconf.dbtable, KEYVALUE_DFT_DBTABLE, CONF_NAME_LEN);
	gconf.debug = KEYVALUE_DFT_DEBUG;
	strncpy(gconf.debug_fname, KEYVALUE_DFT_DEBUG_FNAME, CONF_NAME_LEN);
	strncpy(gconf.xml_fname[0], KEYVALUE_DFT_XML_FNAME, CONF_NAME_LEN);
	gconf.nxml = 1;
}

/******************************************************************************
 * *Function: conf_init
 * *Description: open and parse xml config file
 * *Input: none
 * *Output: none
 * *Return: CONF_SUCCESS/CONF_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int conf_init()
{
	if(xml_open(CONF_FNAME) == PARSE_FAIL)
		return CONF_FAIL;

	if(xml_parse() == PARSE_FAIL)
		return CONF_FAIL;
	
	return CONF_SUCCESS;
}

/******************************************************************************
 * *Function: conf_get_dbname
 * *Description: get dbname and save it into gconf var
 * *Input: none
 * *Output: none
 * *Return: CONF_SUCCESS/CONF_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int conf_get_dbname()
{
	if(xml_get_value(KEYNAME_DBNAME, gconf.dbname, CONF_NAME_LEN) == PARSE_FAIL) {
		strncpy(gconf.dbname, KEYVALUE_DFT_DBNAME, CONF_NAME_LEN);
		return CONF_FAIL;
	}

	return CONF_SUCCESS;
}

/******************************************************************************
 * *Function: conf_get_dbtable
 * *Description: get dbtable and save it into gconf var
 * *Input: none
 * *Output: none
 * *Return: CONF_SUCCESS/CONF_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int conf_get_dbtable()
{
	if(xml_get_value(KEYNAME_DBTABLE, gconf.dbtable, CONF_NAME_LEN) == PARSE_FAIL) {
		strncpy(gconf.dbtable, KEYVALUE_DFT_DBTABLE, CONF_NAME_LEN);
		return CONF_FAIL;
	}

	return CONF_SUCCESS;
}

/******************************************************************************
 * *Function: conf_get_debug
 * *Description: get debug value and save it into gconf var
 * *Input: none
 * *Output: none
 * *Return: CONF_SUCCESS/CONF_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int conf_get_debug()
{
	char buf[CONF_NAME_LEN];

	if(xml_get_value(KEYNAME_DEBUG, buf, CONF_NAME_LEN) == PARSE_FAIL) {
		gconf.debug = KEYVALUE_DFT_DEBUG;
		return CONF_FAIL;
	}

	gconf.debug = atoi(buf);
	if(gconf.debug != DEBUG_CLOSE	||
		gconf.debug != DEBUG_STDOUT	||
		gconf.debug != DEBUG_FILE )
		gconf.debug = KEYVALUE_DFT_DEBUG;

	return CONF_SUCCESS;
}

/******************************************************************************
 * *Function: conf_get_debug_fname
 * *Description: get debug fname and save it into gconf var
 * *Input: none
 * *Output: none
 * *Return: CONF_SUCCESS/CONF_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int conf_get_debug_fname()
{
	if(xml_get_value(KEYNAME_DEBUG_FNAME, gconf.debug_fname, CONF_NAME_LEN) == PARSE_FAIL) {
		strncpy(gconf.debug_fname, KEYVALUE_DFT_DEBUG_FNAME, CONF_NAME_LEN);
		return CONF_FAIL;
	}

	return CONF_SUCCESS;
}

/******************************************************************************
 * *Function: conf_get_debug_fd
 * *Description: get debug file descriptor and save it into gconf var
 * *Input: none
 * *Output: none
 * *Return: CONF_SUCCESS/CONF_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int conf_get_debug_fd()
{
	if(!gconf.debug) {
		gconf.debug_fd = -1;
		return CONF_FAIL;
	} else if(gconf.debug == DEBUG_STDOUT) {
		gconf.debug_fd = STDOUT_FILENO;
		return CONF_SUCCESS;
	} else if(gconf.debug == DEBUG_FILE) {
		gconf.debug_fd = open(gconf.debug_fname, O_RDWR | O_CREAT | O_TRUNC, CONF_FMODE);
		if(gconf.debug_fd == -1) {
			gconf.debug = DEBUG_CLOSE;
			return CONF_FAIL;
		}
		return CONF_SUCCESS;
	}
	
	return CONF_FAIL;
}

/******************************************************************************
 * *Function: conf_get_xml_fname
 * *Description: get xml file name and save it into gconf var
 * *Input: none
 * *Output: none
 * *Return: CONF_SUCCESS/CONF_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int conf_get_xml_fname()
{
	if(xml_get_multi_value(KEYNAME_XML_FNAME, gconf.xml_fname, &gconf.nxml) == PARSE_FAIL) {
		strncpy(gconf.xml_fname[0], KEYVALUE_DFT_XML_FNAME, CONF_NAME_LEN);
		gconf.nxml = 1;
		return CONF_FAIL;
	}

	return CONF_SUCCESS;
}

/******************************************************************************
 * *Function: conf_finish
 * *Description: close xml file
 * *Input: none
 * *Output: none
 * *Return: none
 * *Date: 2016/8/22
 * ****************************************************************************/
void conf_finish()
{
	xml_close();
}

/******************************************************************************
 * *Function: conf_print
 * *Description: print config contents
 * *Input: none
 * *Output: none
 * *Return: none
 * *Date: 2016/8/22
 * ****************************************************************************/
void conf_print()
{
	int i;
	
	if(!gconf.debug)
		return;

	x2s_dbg("dbname = %s\n", gconf.dbname);
	x2s_dbg("dbtable = %s\n", gconf.dbtable);
	if(gconf.debug == DEBUG_CLOSE)
		x2s_dbg("debug = DEBUG_CLOSE(%d)\n", gconf.debug);
	else if(gconf.debug == DEBUG_STDOUT)
		x2s_dbg("debug = DEBUG_STDOUT(%d)\n", gconf.debug);
	else if(gconf.debug == DEBUG_STDOUT)
		x2s_dbg("debug = DEBUG_FILE(%d)\n", gconf.debug);
	else
		x2s_dbg("debug = unkonwn(%d)\n", gconf.debug);
	x2s_dbg("debug_fname = %s\n", gconf.debug_fname);
	x2s_dbg("debug_fd = %d\n", gconf.debug_fd);

	x2s_dbg("xml_fname lists : \n");
	for(i=0; i<gconf.nxml; i++)
		x2s_dbg("%s\n", gconf.xml_fname[i]);
}
