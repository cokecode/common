#ifndef __SHORTCONN_H__
#define __SHORTCONN_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <assert.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "simplelog.h"
#include "atomic.h"

struct shortconn_g {
	int			opt_listen;				/* listen or not? */
	uint16_t	opt_port;				/* source port */
	char		* opt_rhost;			/* remote host */
	uint16_t	opt_rport;				/* remote port */

	int			opt_nthread;			/* # concurrent thread */
	int			opt_packsize;			/* packet size */
	int			opt_udp;				/* udp or not? */
	int			opt_oneshot;			/* start only one connection */
	int			opt_emucalc;			/* emulate app calculation */

	atomic_t	st_nsession;			/* # sessions being completed */
	atomic_t	st_nsession_bad;		/* # sessions being completed */
	double		st_time_begin;			/* in millisecond */
	double		st_time_end;			/* in millisecond */

	int			f_barrier_start;
	int			g_servsock;
};

extern struct shortconn_g g;

int create_udp_server(const char *addr, uint16_t port);
int connect_udp_server(const char *addr, uint16_t port, uint16_t sport);
int create_tcp_server(const char *addr, uint16_t port);
int connect_tcp_server(const char *addr, uint16_t port, uint16_t sport);
void run_client();
void run_server();

double getsystime();

#endif /* !__SHORTCONN_H__ */

/*
 vim:ts=4:sw=4:ai:cindent:tw=78:noet
*/
