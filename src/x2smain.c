#include "xmlparse.h"

int main()
{
	xml_open(XML_NAME);
	xml_parse();
	xml_print();

	xml_save2db();

	xml_close();

	return 0;
}
