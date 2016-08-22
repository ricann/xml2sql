#include "x2sconf.h"
#include "x2sparse.h"
#include "x2sdb.h"
#include "x2smain.h"

/******************************************************************************
 * *Function: main
 * *Description: when generate exe object, use this function to do tests.
 * *Input: none
 * *Output: none
 * *Return: X2S_SUCCESS/X2S_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int main()
{
	int i;
	char tmp[1024];

	conf_read();

	for(i=0; i<gconf.nxml; i++) {
		xml_open(gconf.xml_fname[i]);
		xml_parse_keycat();
		xml_print();
		xml_save2db();
		xml_close();
	}

	db_open();
	db_get_value_str("vin", tmp, 1024);
	db_close();

	return X2S_SUCCESS;
}

/******************************************************************************
 * *Function: x2s_execute
 * *Description: export function, used to parse xml file and save result to db
 * *Input: none
 * *Output: none
 * *Return: X2S_SUCCESS/X2S_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int x2s_execute()
{
	int i;

	conf_read();

	for(i=0; i<gconf.nxml; i++) {
		xml_open(gconf.xml_fname[i]);
		xml_parse_keycat();
		xml_print();
		xml_save2db();
		xml_close();
	}

	return X2S_SUCCESS;
}

