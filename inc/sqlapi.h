#ifndef __SQLAPI_H__
#define __SQLAPI_H__

#define SQLAPI_SUCCESS		0
#define SQLAPI_FAIL			1

#define SQLAPI_DBG			printf

int sql_open(const char *);
int sql_close();
int sql_create_table(const char *);
int sql_delete_table(const char *);
int sql_insert(const char *);
int sql_select(const char *);
int sql_update(const char *);
int sql_delete(const char *);

#endif /* __SQLAPI_H__ */
