#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "x2sdb.h"

static sqlite3 *db;
static char *errmsg;

static int callback(void *notused, int argc, char **argv, char **azColName)
{
	int i;

	for(i=0; i<argc; i++) 
		SQLAPI_DBG("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	SQLAPI_DBG("\n");
	return 0;
}

int sql_open(const char *dbname)
{
	if(!dbname)
		return SQLAPI_FAIL;

	if(sqlite3_open(dbname, &db) != SQLITE_OK) {
		SQLAPI_DBG("open db(%s) fail: %s!\n", dbname, sqlite3_errmsg(db));
		return SQLAPI_FAIL;		
	} 
	
	SQLAPI_DBG("open db(%s) success!\n", dbname);
	return SQLAPI_SUCCESS;
}

int sql_exec(const char *sql)
{
	int rc;

	rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
	if(rc != SQLITE_OK) {
		SQLAPI_DBG("error : %s\n", errmsg);
		sqlite3_free(errmsg);
		return SQLAPI_FAIL;
	}

	SQLAPI_DBG("success!\n");
	return SQLAPI_SUCCESS;
}

int sql_close()
{
	sqlite3_close(db);

	return SQLAPI_SUCCESS;	
}

int sql_key_get(const char *key, char *value, int len)
{
	int nrow;
	int ncol;
	char **result;
	char sqlbuf[1024];

	if(sqlite3_open(DEFAULT_DB_NAME, &db) != SQLITE_OK) {
		SQLAPI_DBG("open db fail: %s!\n", sqlite3_errmsg(db));
		return SQLAPI_FAIL;		
	}

	// search key's value
	snprintf(sqlbuf, sizeof(sqlbuf), SELECT_VALUE_STR, key);
	if(sqlite3_get_table(db, sqlbuf, &result, &nrow, &ncol, &errmsg) != SQLITE_OK) {
		SQLAPI_DBG("sql_key_get error : %s\n", errmsg);
		sqlite3_free(errmsg);
		return SQLAPI_FAIL;
	}

	// handle search result
	SQLAPI_DBG("nrow = %d, ncol = %d\n", nrow, ncol);
	if(nrow < 1)
		return SQLAPI_FAIL;
	strncpy(value, result[0 + ncol], len);
	SQLAPI_DBG("value = %s\n", value);
	
	sql_close();	

	return SQLAPI_SUCCESS;
}
//int sql_key_set(const char *key, char *value, int len)
//int sql_key_del(const char *key)
