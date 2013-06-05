/*
 * Simple macros for debuging.
 * Rev 0.2, 2006-07-17
 * Rev 0.1, 2006-01-18
 * Chen Yongbao <yongbao.chen@net263.com>
 */

#include <stdio.h>

#define LOGLEVEL_NONE		0
#define LOGLEVEL_CRIT		1
#define LOGLEVEL_ERROR		2
#define LOGLEVEL_WARN		3
#define LOGLEVEL_NOTICE		4
#define LOGLEVEL_INFO		5
#define LOGLEVEL_DEBUG		6
#define LOGLEVEL_TRACE		7

// You may define macro 'LOGLEVEL'
// #define LOGLEVEL 1/2/3

// #define LOGLEVEL_DEFAULT	LOGLEVEL_DEBUG

#ifdef USE_VLL
extern int __g_loglevel;
#define simplelog_setlevel(level) \
	do { \
		__g_loglevel = level; \
	} while (0)
#else
#define __g_loglevel 99999
#define simplelog_setlevel(level)
#endif

#define D_SIMPLE_LOG(level, levelname, ...) \
	do { \
		if (__g_loglevel >= level) { \
			printf("[%s][%s:%d:%s]", levelname, __FILE__, __LINE__, __FUNCTION__); \
			printf(__VA_ARGS__); \
			printf("\n"); \
		} \
	} while (0)

#define D_BRIEF_LOG(level, levelname, ...) \
	do { \
		if (__g_loglevel >= level) { \
			printf("[%s][%s:%d]", levelname, __FILE__, __LINE__); \
			printf(__VA_ARGS__); \
			printf("\n"); \
		} \
	} while (0)

#define D_TRACE(...)	D_SIMPLE_LOG(LOGLEVEL_TRACE, "TRACE", __VA_ARGS__)
#define D_DEBUG(...)	D_SIMPLE_LOG(LOGLEVEL_DEBUG, "DEBUG", __VA_ARGS__)
#define D_INFO(...)		D_SIMPLE_LOG(LOGLEVEL_INFO,  "INFO ", __VA_ARGS__)
#define D_NOTICE(...)	D_SIMPLE_LOG(LOGLEVEL_NOTICE,"NOTIC", __VA_ARGS__)
#define D_WARN(...)		D_SIMPLE_LOG(LOGLEVEL_WARN,  "WARN ", __VA_ARGS__)
#define D_ERROR(...)	D_SIMPLE_LOG(LOGLEVEL_ERROR, "ERROR", __VA_ARGS__)
#define D_CRIT(...)		D_SIMPLE_LOG(LOGLEVEL_CRIT,  "CRIT ", __VA_ARGS__)

#define B_TRACE(...)	D_BRIEF_LOG(LOGLEVEL_TRACE, "TRACE", __VA_ARGS__)
#define B_DEBUG(...)	D_BRIEF_LOG(LOGLEVEL_DEBUG, "DEBUG", __VA_ARGS__)
#define B_INFO(...)		D_BRIEF_LOG(LOGLEVEL_INFO,  "INFO ", __VA_ARGS__)
#define B_NOTICE(...)	D_BRIEF_LOG(LOGLEVEL_NOTICE,"NOTIC", __VA_ARGS__)
#define B_WARN(...)		D_BRIEF_LOG(LOGLEVEL_WARN,  "WARN ", __VA_ARGS__)
#define B_ERROR(...)	D_BRIEF_LOG(LOGLEVEL_ERROR, "ERROR", __VA_ARGS__)
#define B_CRIT(...)		D_BRIEF_LOG(LOGLEVEL_CRIT,  "CRIT ", __VA_ARGS__)

/* ------------------------------------------------------------------------ */
/* Example */
#if 0

----- code -----
#include "simplelog.h"

void output_log()
{
	D_TRACE("trace");
	D_DEBUG("debug");
	D_INFO("info");
	D_NOTICE("notice");
	D_WARN("warn");
	D_ERROR("error");
	D_CRIT("crit");
}

#ifdef USE_VLL
int __g_loglevel;	// for simple log
#endif

int main()
{
	printf("==> output all level\n");
	simplelog_setlevel(LOGLEVEL_TRACE);
	output_log();

	printf("==> output none\n");
	simplelog_setlevel(LOGLEVEL_NONE);
	output_log();

	printf("==> output level <= warn\n");
	simplelog_setlevel(LOGLEVEL_WARN);
	output_log();

	return 0;
}
----- code -----

----- compile -----
cc -DUSE_VLL xxx.c

#endif // 0
