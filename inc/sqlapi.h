#ifndef __SQLAPI_H__
#define __SQLAPI_H__

#define SQLAPI_SUCCESS		0
#define SQLAPI_FAIL			1

#define DEFAULT_DB_NAME		"test.db"
#define CREATE_TABLE_STR	"CREATE TABLE IF NOT EXISTS XMLDATA("	\
							"ID INT PRIMARY KEY	 NOT NULL,"	\
							"NAME	VARCHAR(255) NOT NULL,"	\
							"VALUE	VARCHAR(255) NOT NULL,"	\
							"TYPE	INT NOT NULL )" 
#define INSERT_FMT_STR		"INSERT INTO XMLDATA(ID,NAME,VALUE,TYPE) " \
							"VALUES (%d, '%s', '%s', %d); "
#define SELECT_VALUE_STR	"SELECT VALUE FROM XMLDATA WHERE NAME='%s'"

#define SQLAPI_DBG			printf

int sql_open(const char *);
int sql_exec(const char *);
int sql_close();

int sql_key_get(const char *key, char *value, int len);

#endif /* __SQLAPI_H__ */
