#ifndef __X2SCONF_H__
#define __X2SCONF_H__

#define MAX_NAME_LEN		128

enum {
	DEBUG_CLOSE,
	DEBUG_STDOUT,
	DEBUG_FILE,
};

typedef struct glob_conf{
	char dbname[MAX_NAME_LEN];
	char dbtable[MAX_NAME_LEN];

	int debug;
	char debug_fname[MAX_NAME_LEN];
} glob_conf_t;

extern glob_cfg_t gconf;

#endif /* __X2SCONF_H__ */
