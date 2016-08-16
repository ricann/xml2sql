#ifndef __X2SDB_H__
#define __X2SDB_H__

#define DB_SUCCESS		0
#define DB_FAIL			1

#define DB_MAX_STR		1024

// db operation APIs, must call db_open first, and call db_close finally.
int db_open();
int db_close();
int db_get_value_str(const char *key, char *value, int len);
int db_set_value_str(const char *key, char *value, int len);
int db_get_value_int(const char *key, int *value);
int db_set_value_int(const char *key, int value);
int db_insert_item(int id, const char *name, const char *value, int type);
int db_delete_item(const char *name);

// format sql format string routines
char *DBSQL_FMTSTR_CREATE_TABLE(const char *dbtable);
char *DBSQL_FMTSTR_INSERT_ITEM(const char *dbtable, int id, const char *name, const char *value, int type);
char *DBSQL_FMTSTR_DELETE_ITEM(const char *dbtable, const char *name);
char *DBSQL_FMTSTR_UPDATE_VALUE(const char *dbtable, const char *name, const char *value);
char *DBSQL_FMTSTR_SELECT_VALUE(const char *dbtable, const char *name);


#endif /* __X2SDB_H__ */
