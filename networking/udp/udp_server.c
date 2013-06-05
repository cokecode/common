// Simple UDP server.
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef unsigned char uchar;
// typedef unsigned short ushort;
// typedef unsigned int uint;
// typedef unsigned long ulong;

enum {
	OPMODE_DISCARD,			// Discard any data from client
	OPMODE_ECHO,			// Act as an echo server
	OPMODE_WRITE_STDOUT,	// Read from client and write to stdout
};

int create_udp_server(const char *addr, ushort port)
{
	int sfd;
	struct sockaddr_in sa;
	int retval;

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	if (addr != NULL)
		inet_pton(AF_INET, addr, &sa.sin_addr);
	else
		sa.sin_addr.s_addr = INADDR_ANY;

	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sfd == -1) {
		perror("socket");
		return sfd;
	}

	retval = bind(sfd, (struct sockaddr *)&sa, sizeof(sa));
	if (retval == -1) {
		perror("bind");
		close(sfd), sfd = -1;
		return sfd;
	}

	return sfd;
}

int main(int argc, char *argv[])
{
	const char *addr = "0.0.0.0";
	ushort port = 6889;
	int sfd;
	struct sockaddr_in csa;
	socklen_t csalen;
	char buf[8192];
	int bufsize = sizeof(buf);
	int opmode = OPMODE_ECHO;
	int retval;

	if (argc > 1) {
		if (strcasecmp(argv[1], "discard") == 0)
			opmode = OPMODE_DISCARD;
		else if (strcasecmp(argv[1], "echo") == 0)
			opmode = OPMODE_ECHO;
		else if (strcasecmp(argv[1], "-") == 0)
			opmode = OPMODE_WRITE_STDOUT;
		else
			printf("Unknown opmode `%s', assume `echo'.\n", argv[1]);
	}

	sfd = create_udp_server(addr, port);

	for (;;)
	{
		memset(&csa, 0, sizeof(csa));
		csalen = sizeof(csa);
		retval = recvfrom(sfd, buf, bufsize, /*MSG_TRUNC*/ 0,
				(struct sockaddr *)&csa, &csalen);
		if (retval == 0) {
			printf("??? the peer has performed an orderly shutdown\n");
			continue;
		}
		if (retval == -1) {
			perror("recvfrom");
			exit(1);
		}

		if (opmode == OPMODE_DISCARD)
			continue;
		else if (opmode == OPMODE_ECHO) {
			retval = sendto(sfd, buf, retval, MSG_NOSIGNAL, 
					(struct sockaddr *)&csa, csalen);
			if (retval == -1) {
				perror("sendto");
			}
		}
		else if (opmode == OPMODE_WRITE_STDOUT) {
			// buf[retval < bufsize? retval : bufsize - 1] = '\0';
			// printf("[%08x:%d] %s", csa.sin_addr.s_addr, ntohs(csa.sin_port),
					// buf);
			retval = write(STDOUT_FILENO, buf, retval);
			if (retval == -1) {
				perror("write");
				exit(1);
			}
		}
	}

	exit(0);
}
