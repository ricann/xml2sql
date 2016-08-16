#ifndef __X2SMAIN_H__
#define __X2SMAIN_H__

//#if (defined(MACRO_LIBA) || defined(MACRO_LIBSO))
int x2s_execute();

int db_open();
int db_close();
int db_get_value_str(const char *key, char *value, int len);
int db_set_value_str(const char *key, char *value, int len);
int db_get_value_int(const char *key, int *value);
int db_set_value_int(const char *key, int value);
int db_insert_item(int id, const char *name, const char *value, int type);
int db_delete_item(const char *name);

//#endif

#endif /* __X2SMAIN_H__ */
