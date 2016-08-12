#ifndef __X2SCONF_H__
#define __X2SCONF_H__

#define CONF_SUCCESS				0
#define CONF_FAIL					1

#define CONF_NAME_LEN				128

enum {
	DEBUG_CLOSE,
	DEBUG_STDOUT,
	DEBUG_FILE,
};

typedef struct glob_conf{
	char dbname[CONF_NAME_LEN];
	char dbtable[CONF_NAME_LEN];

	int debug;
	char debug_fname[CONF_NAME_LEN];
	int debug_fd;
} glob_conf_t;

extern glob_conf_t gconf;

void conf_read();

#endif /* __X2SCONF_H__ */
