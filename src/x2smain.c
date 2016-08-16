#include "x2sconf.h"
#include "x2sparse.h"

#include "x2sdb.h"

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

	return 0;
}
