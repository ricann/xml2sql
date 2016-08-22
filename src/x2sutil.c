#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include "x2sconf.h"
#include "x2sutil.h"

#define UTIL_STR_LEN		512

/******************************************************************************
 * *Function: x2s_dbg
 * *Description: debug some info into gconf.debug_fd
 * *Input: fmt(debug parameters)
 * *Output: none
 * *Return: none
 * *Date: 2016/8/22
 * ****************************************************************************/
void x2s_dbg(const char *fmt, ...)
{
	int n;
	va_list args;
	char buf[UTIL_STR_LEN];

	if(!gconf.debug)
		return;

	if(gconf.debug_fd < 0)
		return;

	va_start(args, fmt);
	n = vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	// ignore write error
	write(gconf.debug_fd, buf, n);
}

/******************************************************************************
 * *Function: x2s_dbg_exit
 * *Description: debug some info into gconf.debug_fd and exit
 * *Input: fmt(debug parameters)
 * *Output: none
 * *Return: none
 * *Date: 2016/8/22
 * ****************************************************************************/
void x2s_dbg_exit(const char *fmt, ...)
{
	int n;
	va_list args;
	char buf[UTIL_STR_LEN];

	if(!gconf.debug)
		return;

	if(gconf.debug_fd < 0)
		return;

	va_start(args, fmt);
	n = vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	// ignore write error
	write(gconf.debug_fd, buf, n);

	exit(EXIT_FAILURE);
}
