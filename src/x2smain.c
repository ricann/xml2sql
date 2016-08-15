#include "x2sconf.h"
#include "x2sparse.h"

#include "x2sdb.h"

int main()
{
	char tmp[1024];
	conf_read();

	xml_open(XML_NAME);
	xml_parse_keycat();
	xml_print();

	xml_save2db();

	db_open();
	db_get_value("vin", tmp, 1024);
	db_close();

	xml_close();

	return 0;
}
