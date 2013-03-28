/*
 * Simple macros for debuging.
 * Rev 0.1, 2006-01-18
 * Chen Yongbao <yongbao.chen@net263.com>
 */

#define LOGLEVEL_NONE       0
#define LOGLEVEL_CRIT       1
#define LOGLEVEL_ERROR      2
#define LOGLEVEL_WARN       3
#define LOGLEVEL_NOTICE     4
#define LOGLEVEL_INFO       5
#define LOGLEVEL_DEBUG      6
#define LOGLEVEL_TRACE      7

/* Linux header */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

/* FIXME Win32 header */

// You may define macro 'LOGLEVEL'
// #define LOGLEVEL 1/2/3

// #define LOGLEVEL_DEFAULT LOGLEVEL_DEBUG

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
            struct tm __t; \
            struct timeval __tv; \
            gettimeofday(&__tv, NULL); \
            localtime_r(&__tv.tv_sec, &__t); \
            fprintf(stderr, "[%02d:%02d:%02d.%06ld][%s][%s:%d:%s]", \
                __t.tm_hour, __t.tm_min, __t.tm_sec, __tv.tv_usec, \
                levelname, __FILE__, __LINE__, __FUNCTION__); \
            fprintf(stderr, __VA_ARGS__); \
            fprintf(stderr, "\n"); \
        } \
    } while (0)

#define D_DEBUGTS(...) \
    do { \
        if (__g_loglevel >= LOGLEVEL_DEBUG) { \
            struct tm __t; \
            struct timeval __tv; \
            gettimeofday(&__tv, NULL); \
            localtime_r(&__tv.tv_sec, &__t); \
            fprintf(stderr, "[%02d:%02d:%02d.%06ld]", \
                __t.tm_hour, __t.tm_min, __t.tm_sec, __tv.tv_usec); \
            fprintf(stderr, __VA_ARGS__); \
            fprintf(stderr, "\n"); \
        } \
    } while (0)

#define D_TRACE(...)    D_SIMPLE_LOG(LOGLEVEL_TRACE, "TRACE", __VA_ARGS__)
#define D_DEBUG(...)    D_SIMPLE_LOG(LOGLEVEL_DEBUG, "DEBUG", __VA_ARGS__)
#define D_INFO(...)     D_SIMPLE_LOG(LOGLEVEL_INFO,  "INFO ", __VA_ARGS__)
#define D_NOTICE(...)   D_SIMPLE_LOG(LOGLEVEL_NOTICE,"NOTIC", __VA_ARGS__)
#define D_WARN(...)     D_SIMPLE_LOG(LOGLEVEL_WARN,  "WARN ", __VA_ARGS__)
#define D_ERROR(...)    D_SIMPLE_LOG(LOGLEVEL_ERROR, "ERROR", __VA_ARGS__)
#define D_CRIT(...)     D_SIMPLE_LOG(LOGLEVEL_CRIT,  "CRIT ", __VA_ARGS__)

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
int __g_loglevel;   // for simple log
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
/*
 vim:ts=4:sw=4:ai:cindent:et:tw=80
*/
