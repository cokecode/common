#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mainfun.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <pthread.h>

/*
 * level:
 *  0  - empty loop
 *  1  - send + recv
 *  2  - send + select + recv
 *  3  - send + select + recv + gettimeofday
 *  4  - all-of-above, + two processes
 *  5  - all-of-above, + two threads within every process
 *  6  - all-of-above, + memcpy
 */

struct context {
	int sfd;
	char buffer[1024];
	size_t buflen;
	size_t bufmax;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
};

static int g_level = 0;
static int g_loopmax = 100 * 10000;

/* ======================================================================== */

int Socket(int domain, int type, int protocol)
{
	int sfd;
	if ((sfd = socket(domain, type, protocol)) == -1) {
		perror("socket");
		exit(1);
	}
	return sfd;
}

int Bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen)
{
	if (bind(sockfd, my_addr, addrlen) == -1) {
		perror("bind");
		exit(1);
	}
	return 0;
}

int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen)
{
	if (connect(sockfd, serv_addr, addrlen) == -1) {
		perror("connect");
		exit(1);
	}
	return 0;
}

ssize_t Send(int s, const void *msg, size_t len, int flags)
{
	int retval;
	if ((retval = send(s, msg, len, flags)) <= 0) {
		perror("send");
		exit(1);
	}
	return retval;
}

ssize_t Recv(int s, void *buf, size_t len, int flags)
{
	int retval;
	if ((retval = recv(s, buf, len, flags)) <= 0) {
		perror("recv");
		abort();
	}
	return retval;
}

int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
		       struct timeval *timeout)
{
	int retval;
	retval = select(n, readfds, writefds, exceptfds, timeout);
	if (retval == -1) {
		perror("select");
		abort();
	}
	return retval;
}

pid_t Fork()
{
	pid_t pid;
	if ((pid = fork()) == -1) {
		perror("fork");
		abort();
	}
	return pid;
}

int Pthread_create(pthread_t  *  thread, pthread_attr_t * attr, void *
		       (*start_routine)(void *), void * arg)
{
	if (pthread_create(thread, attr, start_routine, arg) != 0) {
		perror("pthread_create");
		abort();
	}
	return 0;
}

int Pthread_mutex_lock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex) != 0) {
		perror("pthread_mutex_lock");
		abort();
	}
	return 0;
}

int Pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_unlock(mutex) != 0) {
		perror("pthread_mutex_unlock");
		abort();
	}
	return 0;
}

int Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	if (pthread_cond_wait(cond, mutex) != 0) {
		perror("pthread_cond_wait");
		abort();
	}
	return 0;
}

int Pthread_cond_signal(pthread_cond_t *cond)
{
	if (pthread_cond_signal(cond) != 0) {
		perror("pthread_cond_signal");
		abort();
	}
	return 0;
}

/* ======================================================================== */

void inline my_time()
{
	if (g_level >= 3) {
		struct timeval tv;
		gettimeofday(&tv, NULL);
		gettimeofday(&tv, NULL);
	}
}

void inline my_send(int sfd)
{
	char buffer[1024];

	if (g_level >= 1)
		Send(sfd, buffer, sizeof(buffer), 0);
}

void inline my_recv(int sfd)
{
	char buffer[1024];

	if (g_level >= 2) {
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(sfd, &fds);
		Select(sfd + 1, &fds, NULL, NULL, 0);
	}

	if (g_level >= 1) {
		Recv(sfd, buffer, sizeof(buffer), 0);
	}
}

void uniproc(int sfd)
{
	int a = 34872342;
	int i, j;
	for (i = 0; i < g_loopmax; i++) {
		my_recv(sfd);
		my_time();

		/* R4-1024 or R4-512 */
		for (j = 0; j < 512; j++) {
			a *= (a + 5.6);
			a <<= 8;
			a *= a / 2.0;
		}
		
		my_send(sfd);
		my_time();
	}
}

void * thrproc_proto(void *);
void * thrproc_app(void *);

void uniproc_mt(int sfd)
{
	struct context ctx;
	pthread_t tid;

	ctx.sfd = sfd;
	ctx.buflen = 0;
	ctx.bufmax = sizeof(ctx.buffer);
	pthread_mutex_init(&ctx.mutex, NULL);
	pthread_cond_init(&ctx.cond, NULL);

	Pthread_create(&tid, NULL, thrproc_proto, &ctx);

	thrproc_app(&ctx);
}

void * thrproc_app(void * arg)
{
	struct context * ctx = (struct context *)arg;
	int sfd = ctx->sfd;
	char buf[1024];
	int a = 2384723;
	int i, j;

	for (i = 0; i < g_loopmax; i++) {
		Pthread_mutex_lock(&ctx->mutex);
		if (ctx->buflen > 0) {
			if (g_level >= 6) {
				memcpy(buf, ctx->buffer, sizeof(buf));
				memcpy(ctx->buffer, buf, sizeof(buf));
			}

			/* R6-1024 or R6-512 */
			for (j = 0; j < 512; j++) {
				a *= (a + 5.6);
				a <<= 8;
				a *= a / 2.0;
			}

			Send(sfd, ctx->buffer, ctx->buflen, 0);
			ctx->buflen = 0;
		}

		Pthread_cond_wait(&ctx->cond, &ctx->mutex);
		Pthread_mutex_unlock(&ctx->mutex);
	}

	return NULL;
}

void * thrproc_proto(void * arg)
{
	struct context * ctx = (struct context *)arg;
	fd_set fds;
	int sfd = ctx->sfd;
	int retval;
	int i;

	for (i = 0; i < g_loopmax + 1; i++) {
		my_time();

		FD_ZERO(&fds);
		FD_SET(sfd, &fds);
		Select(sfd + 1, &fds, NULL, NULL, NULL);

		my_time();
		Pthread_mutex_lock(&ctx->mutex);
		for (;;) {
			retval = Recv(sfd, ctx->buffer, ctx->bufmax, 0);
			if (retval > 0) {
				ctx->buflen = retval;
				break;
			}
		}
		Pthread_cond_signal(&ctx->cond);
		Pthread_mutex_unlock(&ctx->mutex);
	}

	return NULL;
}

void simple_proc(int SocketSendID, int SocketRecvID)
{
	int i;
	for(i=0; i<g_loopmax; i++)
	{
		my_send(SocketSendID);
		my_time();

		my_recv(SocketRecvID);
		my_time();
		my_send(SocketRecvID);
		my_time();

		my_recv(SocketSendID);
		my_time();
	}
	// printf("ok!\n");	
}

int main (int argc, char *argv[])
{
	int   SendPort = 6000, RecvPort = 7000;

	if (argc > 1)
		g_level = atoi(argv[1]);
	
	//create socket
    int SocketSendID,  SocketRecvID;
	SocketSendID = Socket(AF_INET, SOCK_DGRAM, 0);
	SocketRecvID = Socket(AF_INET, SOCK_DGRAM, 0);
	
	//set socket address information
	struct sockaddr_in  SocketSend,   SocketRecv;	  

	memset(&SocketSend, 0, sizeof(SocketSend));
	memset(&SocketRecv, 0, sizeof(SocketRecv));
	SocketSend.sin_family = AF_INET;
	SocketRecv.sin_family = AF_INET;
	SocketSend.sin_port = htons(SendPort);
	SocketRecv.sin_port = htons(RecvPort);
	SocketSend.sin_addr.s_addr = INADDR_ANY;
	SocketRecv.sin_addr.s_addr = INADDR_ANY;
	//socket_send.sin_addr = *(in_addr *)(hostdata->h_addr);

	//bind socket
	Bind(SocketSendID, (struct sockaddr *)&SocketSend, sizeof(struct sockaddr_in));		
	Bind(SocketRecvID, (struct sockaddr *)&SocketRecv, sizeof(struct sockaddr_in));

	Connect(SocketSendID, (struct sockaddr *)&SocketRecv, sizeof(struct sockaddr_in));
	Connect(SocketRecvID, (struct sockaddr *)&SocketSend, sizeof(struct sockaddr_in));		

	if (g_level >= 4) {
		/* first push */
		Send(SocketSendID, "a", 1, 0);

		if (Fork() == 0) {	/* child 1 */
			close(SocketRecvID), SocketRecvID = -1;
			if (g_level >= 5)
				uniproc_mt(SocketSendID);
			else
				uniproc(SocketSendID);
			exit(0);
		}
		if (fork() == 0) {	/* child 2 */
			close(SocketSendID), SocketSendID = -1;
			if (g_level >= 5)
				uniproc_mt(SocketRecvID);
			else
				uniproc(SocketRecvID);
			exit(0);
		}

		/* parent, wait only one child */
		// wait(NULL);
		while (wait(NULL) > 0) {
			kill(0, SIGINT);
		}
	}
	else {
		simple_proc(SocketSendID, SocketRecvID);
	}

	return 0;
}
	
