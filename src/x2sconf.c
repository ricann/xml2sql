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

#define	KEYVALUE_DFT_DBNAME			"x2s.db" 
#define	KEYVALUE_DFT_DBTABLE		"X2STABLE" 
#define	KEYVALUE_DFT_DEBUG			DEBUG_STDOUT
#define	KEYVALUE_DFT_DEBUG_FNAME	"x2s.dbg" 

glob_conf_t gconf;

static void conf_default();
static int conf_init();
static int conf_get_dbname();
static int conf_get_dbtable();
static int conf_get_debug();
static int conf_get_debug_fname();
static int conf_get_debug_fd();
static void conf_finish();
static void conf_print();

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

	// close xml file
	conf_finish();

	// for debug
	conf_print();

	return;
}

void conf_default()
{
printf("1.0.0.1\n");
	strncpy(gconf.dbname, KEYVALUE_DFT_DBNAME, CONF_NAME_LEN);
printf("1.0.0.2\n");
	strncpy(gconf.dbtable, KEYVALUE_DFT_DBTABLE, CONF_NAME_LEN);
printf("1.0.0.3\n");
	gconf.debug = KEYVALUE_DFT_DEBUG;
	strncpy(gconf.debug_fname, KEYVALUE_DFT_DEBUG_FNAME, CONF_NAME_LEN);
printf("1.0.0.4\n");
}

int conf_init()
{
	if(xml_open(CONF_FNAME) == PARSE_FAIL)
		return CONF_FAIL;

	if(xml_parse() == PARSE_FAIL)
		return CONF_FAIL;
	
	return CONF_SUCCESS;
}

int conf_get_dbname()
{
	if(xml_get_key(KEYNAME_DBNAME, gconf.dbname, CONF_NAME_LEN) == PARSE_FAIL) {
		strncpy(gconf.dbname, KEYVALUE_DFT_DBNAME, CONF_NAME_LEN);
		return CONF_FAIL;
	}

	return CONF_SUCCESS;
}

int conf_get_dbtable()
{
	if(xml_get_key(KEYNAME_DBTABLE, gconf.dbtable, CONF_NAME_LEN) == PARSE_FAIL) {
		strncpy(gconf.dbtable, KEYVALUE_DFT_DBTABLE, CONF_NAME_LEN);
		return CONF_FAIL;
	}

	return CONF_SUCCESS;
}

int conf_get_debug()
{
	char buf[CONF_NAME_LEN];

	if(xml_get_key(KEYNAME_DEBUG, buf, CONF_NAME_LEN) == PARSE_FAIL) {
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

int conf_get_debug_fname()
{
	if(xml_get_key(KEYNAME_DEBUG_FNAME, gconf.debug_fname, CONF_NAME_LEN) == PARSE_FAIL) {
		strncpy(gconf.debug_fname, KEYVALUE_DFT_DEBUG_FNAME, CONF_NAME_LEN);
		return CONF_FAIL;
	}

	return CONF_SUCCESS;
}

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

void conf_finish()
{
	xml_close();
}

void conf_print()
{
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
}
