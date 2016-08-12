#include "x2sconf.h"
#include "x2sparse.h"
#include "sqlapi.h"

int main()
{
	char tmp[1024];
	conf_read();

	xml_open(XML_NAME);
	xml_parse_keycat();
	xml_print();

	xml_save2db();

	sql_key_get("vin", tmp, 1024);

	xml_close();

	return 0;
}
