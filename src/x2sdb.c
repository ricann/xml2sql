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

// internal used functions
static int db_exec_sql(const char *);
static int callback(void *, int , char **, char **);

/******************************************************************************
 * *Function: callback
 * *Description: sqlite API callback function, used to get related info when 
 * * *operatiing database. 
 * *Date: 2016/8/22
 * ****************************************************************************/
static int callback(void *notused, int argc, char **argv, char **azColName)
{
	int i;

	for(i=0; i<argc; i++) 
		x2s_dbg("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	x2s_dbg("\n");
	return 0;
}

/******************************************************************************
 * *Function: db_exec_sql
 * *Description: execute sql statement
 * *Input: sql(sql statement)
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
static int db_exec_sql(const char *sql)
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

/******************************************************************************
 * *Function: db_open
 * *Description: open sqlite database
 * *Input: none
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_open()
{
	if(!gconf.dbname)
		return DB_FAIL;

	if(sqlite3_open(gconf.dbname, &db) != SQLITE_OK) {
		x2s_dbg("open db(%s) fail: %s!\n", gconf.dbname, sqlite3_errmsg(db));
		return DB_FAIL;		
	} 
	
	x2s_dbg("open db(%s) success!\n", gconf.dbname);
	return DB_SUCCESS;
}

/******************************************************************************
 * *Function: db_close
 * *Description: close sqlite database
 * *Input: none
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_close()
{
	sqlite3_close(db);
	db = NULL;

	return DB_SUCCESS;	
}

/******************************************************************************
 * *Function: db_get_value_str
 * *Description: give a key, get its value from db
 * *Input: key(db priamry key), len(value's buffer length)
 * *Output: value(key's value)
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_get_value_str(const char *key, char *value, int len)
{
	int nrow;
	int ncol;
	char *selsql;
	char **result;

	// get sql format string
	selsql = DBSQL_FMTSTR_SELECT_VALUE(gconf.dbtable, key);
	if(!selsql) {
		x2s_dbg("DBSQL_FMTSTR_SELECT_VALUE error!\n");
		return DB_FAIL;
	}
	// search key's value
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
	
	return DB_SUCCESS;
}

/******************************************************************************
 * *Function: db_set_value_str
 * *Description: give a key, set its value 
 * *Input: key(db priamry key), value(key's value), len(value's length)
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_set_value_str(const char *key, char *value, int len)
{
	char *fmtsql;

	// get sql update format string
	fmtsql = DBSQL_FMTSTR_UPDATE_VALUE(gconf.dbtable, key, value);
	if(!fmtsql) {
		x2s_dbg("DBSQL_FMTSTR_UPDATE_VALUE error\n");
		return DB_FAIL;
	}

	if(db_exec_sql(fmtsql) == DB_FAIL) {
		x2s_dbg("sql_exec %s fail!\n", fmtsql);
		return DB_FAIL;
	}

	return DB_SUCCESS;
}

/******************************************************************************
 * *Function: db_get_value_int
 * *Description: give a key, get its value to a integer variable 
 * *Input: key(db priamry key)
 * *Output: value(key's value)
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_get_value_int(const char *key, int *value)
{
	int nrow;
	int ncol;
	char *selsql;
	char **result;
	char buf[128];

	// get sql format string
	selsql = DBSQL_FMTSTR_SELECT_VALUE(gconf.dbtable, key);
	if(!selsql) {
		x2s_dbg("DBSQL_FMTSTR_SELECT_VALUE error!\n");
		return DB_FAIL;
	}
	// search key's value
	if(sqlite3_get_table(db, selsql, &result, &nrow, &ncol, &errmsg) != SQLITE_OK) {
		x2s_dbg("sql_key_get error : %s\n", errmsg);
		sqlite3_free(errmsg);
		return DB_FAIL;
	}

	// handle search result
	x2s_dbg("nrow = %d, ncol = %d\n", nrow, ncol);
	if(nrow < 1)
		return DB_FAIL;
	strncpy(buf, result[0 + ncol], sizeof(buf));
	*value = atoi(buf);
	x2s_dbg("value = %d\n", *value);
	
	return DB_SUCCESS;
}

/******************************************************************************
 * *Function: db_set_value_int
 * *Description: give a key, set its value using a integer variable 
 * *Input: key(db priamry key), value(key's value)
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_set_value_int(const char *key, int value)
{
	char *fmtsql;
	char buf[128];

	// get sql update format string
	snprintf(buf, sizeof(buf), "%d", value);
	fmtsql = DBSQL_FMTSTR_UPDATE_VALUE(gconf.dbtable, key, buf);
	if(!fmtsql) {
		x2s_dbg("DBSQL_FMTSTR_UPDATE_VALUE error\n");
		return DB_FAIL;
	}

	if(db_exec_sql(fmtsql) == DB_FAIL) {
		x2s_dbg("sql_exec %s fail!\n", fmtsql);
		return DB_FAIL;
	}

	return DB_SUCCESS;
}

/******************************************************************************
 * *Function: db_insert_item
 * *Description: insert an item of record into db
 * *Input: id(record's id), name(primary key name), value(key's value), 
 * * *type(record type)
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_insert_item(int id, const char *name, const char *value, int type)
{
	char *fmtsql;

	// create table
	fmtsql = DBSQL_FMTSTR_CREATE_TABLE(gconf.dbtable);
	if(!fmtsql) {
		x2s_dbg("DBSQL_FMTSTR_CREATE_TABLE error!\n");
		return DB_FAIL;
	}
	db_exec_sql(fmtsql);

	// insert record item
	fmtsql = DBSQL_FMTSTR_INSERT_ITEM(gconf.dbtable, id, name, value, type);
	if(!fmtsql) {
		x2s_dbg("DBSQL_FMTSTR_INSERT_ITEM error!\n");
		return DB_FAIL;
	}
	db_exec_sql(fmtsql);

	return DB_SUCCESS;
}

/******************************************************************************
 * *Function: db_delete_item
 * *Description: delete an item of record from db
 * *Input: name(primary key name)
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_delete_item(const char *name)
{
	char *fmtsql;

	// insert record item
	fmtsql = DBSQL_FMTSTR_DELETE_ITEM(gconf.dbtable, name);
	if(!fmtsql) {
		x2s_dbg("DBSQL_FMTSTR_INSERT_ITEM error!\n");
		return DB_FAIL;
	}
	db_exec_sql(fmtsql);

	return DB_SUCCESS;
}

/******************************************************************************
 * *Function: DBSQL_FMTSTR_CREATE_TABLE
 * *Description: make a sql statement string used to create a table 
 * *Input: dbtable(database table name)
 * *Output: none
 * *Return: sql statement string
 * *Date: 2016/8/22
 * ****************************************************************************/
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

/******************************************************************************
 * *Function: DBSQL_FMTSTR_INSERT_ITEM
 * *Description: make a sql statement string used to insert a record item
 * *Input: dbtable(database table name) ,id(record's id), 
 * * *name(primary key name), value(key's value), type(record type)
 * *Output: none
 * *Return: sql statement string
 * *Date: 2016/8/22
 * ****************************************************************************/
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

/******************************************************************************
 * *Function: DBSQL_FMTSTR_DELETE_ITEM
 * *Description: make a sql statement string used to delete a record item
 * *Input: dbtable(database table name), name(primary key name)
 * *Output: none
 * *Return: sql statement string
 * *Date: 2016/8/22
 * ****************************************************************************/
char *DBSQL_FMTSTR_DELETE_ITEM(const char *dbtable, const char *name)
{
	int n;

	if(!dbtable || !name )
		return NULL;

	memset(sqlstr, 0, sizeof(sqlstr));
	n = snprintf(sqlstr, DB_MAX_STR,	\
		"DELETE FROM %s WHERE NAME='%s'",		\
		dbtable, name);

	if(n < 0)
		return NULL;

	return sqlstr;

}

/******************************************************************************
 * *Function: DBSQL_FMTSTR_UPDATE_VALUE
 * *Description: make a sql statement string used to update a record item
 * *Input: dbtable(database table name), name(primary key name), 
 * * *value(key name's value)
 * *Output: none
 * *Return: sql statement string
 * *Date: 2016/8/22
 * ****************************************************************************/
char *DBSQL_FMTSTR_UPDATE_VALUE(const char *dbtable, const char *name, const char *value)
{
	int n;

	if(!dbtable || !name )
		return NULL;

	memset(sqlstr, 0, sizeof(sqlstr));
	n = snprintf(sqlstr, DB_MAX_STR,	\
		"UPDATE %s set VALUE='%s' where NAME='%s'; ",	\
		dbtable, value, name);

	if(n < 0)
		return NULL;

	return sqlstr;

}

/******************************************************************************
 * *Function: DBSQL_FMTSTR_SELECT_VALUE
 * *Description: make a sql statement string used to select a record item
 * *Input: dbtable(database table name), name(primary key name), 
 * *Output: none
 * *Return: sql statement string
 * *Date: 2016/8/22
 * ****************************************************************************/
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

