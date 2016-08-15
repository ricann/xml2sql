#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "x2sutil.h"
#include "x2sconf.h"
#include "x2sdb.h"

static sqlite3 *db;
static char *errmsg;

// for saving formatted sql string 
static char sqlstr[DB_MAX_STR];

static int callback(void *notused, int argc, char **argv, char **azColName)
{
	int i;

	for(i=0; i<argc; i++) 
		x2s_dbg("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	x2s_dbg("\n");
	return 0;
}

int sql_open(const char *dbname)
{
	if(!dbname)
		return DB_FAIL;

	if(sqlite3_open(dbname, &db) != SQLITE_OK) {
		x2s_dbg("open db(%s) fail: %s!\n", dbname, sqlite3_errmsg(db));
		return DB_FAIL;		
	} 
	
	x2s_dbg("open db(%s) success!\n", dbname);
	return DB_SUCCESS;
}

int sql_exec(const char *sql)
{
	int rc;

	rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
	if(rc != SQLITE_OK) {
		x2s_dbg("error : %s\n", errmsg);
		sqlite3_free(errmsg);
		return DB_FAIL;
	}

	x2s_dbg("success!\n");
	return DB_SUCCESS;
}

int sql_close()
{
	sqlite3_close(db);

	return DB_SUCCESS;	
}

int sql_key_get(const char *key, char *value, int len)
{
	int nrow;
	int ncol;
	char *selsql;
	char **result;

	if(sqlite3_open(gconf.dbname, &db) != SQLITE_OK) {
		x2s_dbg("open db fail: %s!\n", sqlite3_errmsg(db));
		return DB_FAIL;		
	}

	// search key's value
	selsql = DBSQL_FMTSTR_SELECT_VALUE(gconf.dbtable, key);
	if(!selsql) {
		x2s_dbg("DBSQL_FMTSTR_SELECT_VALUE error!\n");
		return DB_FAIL;
	}
	if(sqlite3_get_table(db, selsql, &result, &nrow, &ncol, &errmsg) != SQLITE_OK) {
		x2s_dbg("sql_key_get error : %s\n", errmsg);
		sqlite3_free(errmsg);
		return DB_FAIL;
	}

	// handle search result
	x2s_dbg("nrow = %d, ncol = %d\n", nrow, ncol);
	if(nrow < 1)
		return DB_FAIL;
	strncpy(value, result[0 + ncol], len);
	x2s_dbg("value = %s\n", value);
	
	sql_close();	

	return DB_SUCCESS;
}
//int sql_key_set(const char *key, char *value, int len)
//int sql_key_del(const char *key)
//

char *DBSQL_FMTSTR_CREATE_TABLE(const char *dbtable)
{
	int n;

	if(!dbtable)
		return NULL;

	memset(sqlstr, 0, sizeof(sqlstr));
	n = snprintf(sqlstr, DB_MAX_STR, \
		"CREATE TABLE IF NOT EXISTS %s("	\
		"ID INT NOT NULL,"	\
		"NAME VARCHAR(255) PRIMARY KEY NOT NULL,"	\
		"VALUE VARCHAR(255) NOT NULL,"	\
		"TYPE INT NOT NULL )" ,	\
		dbtable);

	if(n < 0)
		return NULL;

	return sqlstr;
}

char *DBSQL_FMTSTR_INSERT_ITEM(const char *dbtable, int id, 
		const char *name, const char *value, int type)
{
	int n;

	if(!dbtable || !name || !value)
		return NULL;

	memset(sqlstr, 0, sizeof(sqlstr));
	n = snprintf(sqlstr, DB_MAX_STR,	\
		"INSERT INTO %s(ID,NAME,VALUE,TYPE) " \
		"VALUES (%d, '%s', '%s', %d); ",	\
		dbtable, id, name, value, type);

	if(n < 0)
		return NULL;

	return sqlstr;
}

char *DBSQL_FMTSTR_SELECT_VALUE(const char *dbtable, const char *name)
{
	int n;

	if(!dbtable || !name )
		return NULL;

	memset(sqlstr, 0, sizeof(sqlstr));
	n = snprintf(sqlstr, DB_MAX_STR,	\
		"SELECT VALUE FROM %s WHERE NAME='%s'",		\
		dbtable, name);

	if(n < 0)
		return NULL;

	return sqlstr;

}
