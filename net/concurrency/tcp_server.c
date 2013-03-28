// Simple TCP server.
// yongbao.chen@net263.com
// 2006-01-19
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "simplelog.h"

typedef unsigned char uchar;
// typedef unsigned short ushort;
// typedef unsigned int uint;
// typedef unsigned long ulong;

static int create_tcp_server(const char *addr, ushort port)
{
	int sfd;
	struct sockaddr_in sa;
	int optval;

	memset(&sa, 0, sizeof(sa));
	sa.sin_port = htons(port);
	sa.sin_family = AF_INET;
	if (addr != NULL)
		inet_pton(AF_INET, addr, &sa.sin_addr);
	else
		sa.sin_addr.s_addr = INADDR_ANY;

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd == -1) {
		perror("socket");
		return sfd;
	}

	optval = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	if (bind(sfd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
		perror("bind");
		close(sfd), sfd = -1;
		return -1;
	}

	if (listen(sfd, 128) == -1) {
		perror("listen");
		close(sfd), sfd = -1;
		return -1;
	}

	return sfd;
}

// return value in millisecond
static double getsystime()
{
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);
	return (double)tv.tv_sec * 1000.0f + (double)tv.tv_usec / 1000.0;
}

static int handle_client(int cfd)
{
	struct sockaddr_in sa;
	socklen_t salen;
	char buf[188 + 4];
	int retval;

	memset(&sa, 0, sizeof(sa));
	salen = sizeof(sa);
	getpeername(cfd, (struct sockaddr *)&sa, &salen);

	int rate = 1024 * 1024;
	long long tx_total = 0;
	double begintime;
	double currtime;
	int currrate;

	begintime = getsystime();

	for (;;)
	{
		retval = write(cfd, buf, sizeof(buf));
		if (retval != sizeof(buf))
			abort();

		tx_total += retval;
		currtime = getsystime();
		currrate = (int)(tx_total * 8 * 1000 / (currtime - begintime));

		// printf("rate/currrate = %d/%d\n", rate, currrate);
		if (currrate >= rate)
			usleep(10);
	}

	return 0;
}

void sighdl(int signum)
{
	// If children exit at the same time, it seems that the system merges
	// several SIGCHLD signal to one.
	// This is a simple way to solve the question.
	if (signum == SIGCHLD) {
		int i;
		for (i = 0; i < 10; i++) {
			waitpid(-1, NULL, WNOHANG);
			sleep(0);
		}
	}

	return;
}

int main(int argc, char *argv[])
{
	const char *addr = "0.0.0.0";
	ushort port = 6889;
	int sfd = -1;
	int cfd = -1;
	struct sockaddr_in sa;
	socklen_t salen = sizeof(sa);
	int subpid;

	if (argc >= 3) {
		addr = argv[1];
		port = atoi(argv[2]);
	}

	signal(SIGCHLD, sighdl);

	sfd = create_tcp_server(addr, port);
	if (sfd == -1)
		exit(1);

	D_INFO("server listen on port %d", port);

	for (;;)
	{
		cfd = accept(sfd, (struct sockaddr *)&sa, &salen);
		if (cfd == -1) {
			if (errno == EINTR)
				continue;
			perror("accept");
			exit(1);
		}
		D_DEBUG("client [%08x:%d] connected.", sa.sin_addr.s_addr,
				ntohs(sa.sin_port));

		subpid = fork();
		if (subpid == -1) { // error occured
			perror("fork");
		}
		else if (subpid == 0) { // child
			close(sfd), sfd = -1;
			handle_client(cfd);
			close(cfd), cfd = -1;
			exit(0);
		}
		else { // parent
			// do nothing
		}

		close(cfd), cfd = -1;
	}

	exit(0);
}
