#ifndef __X2SDB_H__
#define __X2SDB_H__

#define DB_SUCCESS		0
#define DB_FAIL			1

#define DB_MAX_STR		1024


int sql_open(const char *);
int sql_exec(const char *);
int sql_close();

int sql_key_get(const char *key, char *value, int len);

// format sql format string routines
char *DBSQL_FMTSTR_CREATE_TABLE(const char *dbtable);
char *DBSQL_FMTSTR_INSERT_ITEM(const char *dbtable, int id, 
		const char *name, const char *value, int type);
char *DBSQL_FMTSTR_SELECT_VALUE(const char *dbtable, const char *name);


#endif /* __X2SDB_H__ */
