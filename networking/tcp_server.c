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

enum {
	OPMODE_DISCARD,			// Discard any data from client
	OPMODE_ECHO,			// Act as an echo server
	OPMODE_WRITE_STDOUT,	// Read from client and write to stdout
};

// TODO
enum {
	SRVMODE_NONE,			// Unused
	SRVMODE_SINGLE,			// One connection one time
	SRVMODE_MP,				// Simple multi-process mode
	SRVMODE_MP_POOL,		// Multi-process pool mode
	SRVMODE_MP_POOL2,		// Multi-process pool mode 2
	SRVMODE_MT,				// Simple multi-thread mode
	SRVMODE_MT_POOL,		// Multi-thread pool mode
	SRVMODE_MT_POOL2,		// Multi-thread pool mode 2
	SRVMODE_MPED,			// Multi-process with polling
	SRVMODE_MTED,			// Multi-thread with polling
};

static int g_srvmode = SRVMODE_MP;

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

static int handle_client(int cfd, int opmode)
{
	struct sockaddr_in sa;
	socklen_t salen;
	char buf[8192];
	int bufsize = sizeof(buf);
	int retval;

	memset(&sa, 0, sizeof(sa));
	salen = sizeof(sa);
	getpeername(cfd, (struct sockaddr *)&sa, &salen);

	D_DEBUG("opmode = %d", opmode);

	for (;;)
	{
		retval = read(cfd, buf, bufsize);
		if (retval == 0) {
			D_DEBUG("client [%08x:%d] disconnected.\n",
					sa.sin_addr.s_addr, ntohs(sa.sin_port));
			break;
		}

		if (retval == -1) {
			if (errno == EINTR)
				continue;
			perror("read socket failed");
			break;
		}
		// printf("read ok\n");

		if (opmode == OPMODE_DISCARD)
			continue;
		else if (opmode == OPMODE_ECHO) {
			retval = write(cfd, buf, retval);
			if (retval == -1) {
				perror("write socket failed");
				break;
			}
			// printf("write ok\n");
		}
		else if (opmode == OPMODE_WRITE_STDOUT) {
			retval = write(STDOUT_FILENO, buf, retval);
			if (retval == -1) {
				if (errno == EINTR)
					continue;
				perror("write stdout failed");
				exit(1);
			}
		}
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
	int opmode = OPMODE_ECHO;

	if (argc > 1) {
		if (strcmp(argv[1], "-") == 0)
			opmode = OPMODE_WRITE_STDOUT;
		else if (strcasecmp(argv[1], "discard") == 0)
			opmode = OPMODE_DISCARD;
		else if (strcasecmp(argv[1], "echo") == 0)
			opmode = OPMODE_ECHO;
		else
			D_DEBUG("Unknown opmode `%s', assume `echo'.\n", argv[1]);
	}

	D_DEBUG("opmode = %d", opmode);
	D_DEBUG("srvmode = %d", g_srvmode);

	signal(SIGCHLD, sighdl);

	sfd = create_tcp_server(addr, port);
	if (sfd == -1)
		exit(1);

	for (;;)
	{
		cfd = accept(sfd, (struct sockaddr *)&sa, &salen);
		if (cfd == -1) {
			if (errno == EINTR)
				continue;
			perror("accept");
			exit(1);
		}
		D_DEBUG("client [%08x:%d] connected.\n", sa.sin_addr.s_addr,
				ntohs(sa.sin_port));

		if (g_srvmode == SRVMODE_SINGLE)
		{
			handle_client(cfd, opmode);
		}
		else if (g_srvmode == SRVMODE_MP)
		{
			int subpid = fork();
			if (subpid == -1) { // error occured
				perror("fork");
			}
			else if (subpid == 0) { // child
				close(sfd), sfd = -1;
				handle_client(cfd, opmode);
				close(cfd), cfd = -1;
				exit(0);
			}
			else { // parent
				// do nothing
			}
		}

		close(cfd), cfd = -1;
	}

	exit(0);
}
