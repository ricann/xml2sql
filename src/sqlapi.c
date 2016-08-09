#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "sqlapi.h"

static sqlite3 *db;
static char *errmsg;

static int callback(void *notused, int argc, char **argv, char **azColName);

int sql_open(const char *dbname)
{
	int rc;

	if(!dbname)
		return SQLAPI_FAIL;

	rc = sqlite3_open(dbname, &db);	
	if(rc) {
		SQLAPI_DBG("open db(%s) fail: %s!\n", dbname, sqlite3_errmsg(db));
		return SQLAPI_FAIL;		
	} 
	
	SQLAPI_DBG("open db(%s) success!\n", dbname);
	return SQLAPI_SUCCESS;
}

int sql_close()
{
	sqlite3_close(db);

	return SQLAPI_SUCCESS;	
}

int sql_create_table(const char *sql)
{
	int rc;

	rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
	if(rc != SQLITE_OK) {
		SQLAPI_DBG("create table error: %s\n", errmsg);
		return SQLAPI_FAIL;
	}

	SQLAPI_DBG("create table success!\n");
	return SQLAPI_SUCCESS;
}

int sql_insert(const char *sql)
{
	int rc;

	rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
	if(rc != SQLITE_OK) {
		SQLAPI_DBG("insert error: %s\n", errmsg);
		return SQLAPI_FAIL;
	}

	SQLAPI_DBG("insert success!\n");
	return SQLAPI_SUCCESS;
}

int sql_select(const char *sql)
{
	int rc;

	rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
	if(rc != SQLITE_OK) {
		SQLAPI_DBG("select error: %s\n", errmsg);
		return SQLAPI_FAIL;
	}

	SQLAPI_DBG("select success!\n");
	return SQLAPI_SUCCESS;
}

int sql_update(const char *sql)
{
	int rc;

	rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
	if(rc != SQLITE_OK) {
		SQLAPI_DBG("update error: %s\n", errmsg);
		return SQLAPI_FAIL;
	}

	SQLAPI_DBG("update success!\n");
	return SQLAPI_SUCCESS;
}

int sql_delete(const char *sql)
{
	int rc;

	rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
	if(rc != SQLITE_OK) {
		SQLAPI_DBG("delete error: %s\n", errmsg);
		return SQLAPI_FAIL;
	}

	SQLAPI_DBG("delete success!\n");
	return SQLAPI_SUCCESS;
}

static int callback(void *notused, int argc, char **argv, char **azColName)
{
	int i;

	for(i=0; i<argc; i++) 
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	printf("\n");
	return 0;
}
