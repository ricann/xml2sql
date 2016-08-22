#ifndef __X2SMAIN_H__
#define __X2SMAIN_H__

#define X2S_SUCCESS		0
#define X2S_FAIL		1

/******************************************************************************
 * *Function: main
 * *Description: when generate exe object, use this function to do tests.
 * *Input: none
 * *Output: none
 * *Return: X2S_SUCCESS/X2S_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int x2s_execute();

/******************************************************************************
 * *Function: db_open
 * *Description: open sqlite database
 * *Input: none
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_open();

/******************************************************************************
 * *Function: db_close
 * *Description: close sqlite database
 * *Input: none
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_close();

/******************************************************************************
 * *Function: db_get_value_str
 * *Description: give a key, get its value from db
 * *Input: key(db priamry key), len(value's buffer length)
 * *Output: value(key's value)
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_get_value_str(const char *key, char *value, int len);

/******************************************************************************
 * *Function: db_set_value_str
 * *Description: give a key, set its value 
 * *Input: key(db priamry key), value(key's value), len(value's length)
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_set_value_str(const char *key, char *value, int len);

/******************************************************************************
 * *Function: db_get_value_int
 * *Description: give a key, get its value to a integer variable 
 * *Input: key(db priamry key)
 * *Output: value(key's value)
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_get_value_int(const char *key, int *value);

/******************************************************************************
 * *Function: db_set_value_int
 * *Description: give a key, set its value using a integer variable 
 * *Input: key(db priamry key), value(key's value)
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_set_value_int(const char *key, int value);

/******************************************************************************
 * *Function: db_insert_item
 * *Description: insert an item of record into db
 * *Input: id(record's id), name(primary key name), value(key's value), 
 * * *type(record type)
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_insert_item(int id, const char *name, const char *value, int type);

/******************************************************************************
 * *Function: db_delete_item
 * *Description: delete an item of record from db
 * *Input: name(primary key name)
 * *Output: none
 * *Return: DB_SUCCESS/DB_FAIL
 * *Date: 2016/8/22
 * ****************************************************************************/
int db_delete_item(const char *name);


#endif /* __X2SMAIN_H__ */
