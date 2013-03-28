// Simple UDP client.
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
	OPMODE_READ_STDIN,		// Read from stdin and send to server
							// (like ftp upload)
};

int connect_udp_server(const char *addr, ushort port)
{
	int cfd;
	struct sockaddr_in sa;
	int retval;

	if (addr == NULL)
		return -1;

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	inet_pton(AF_INET, addr, &sa.sin_addr);

	cfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (cfd == -1) {
		perror("socket");
		return cfd;
	}

	retval = connect(cfd, (struct sockaddr *)&sa, sizeof(sa));
	if (retval == -1) {
		perror("connect");
		close(cfd), cfd = -1;
		return cfd;
	}
	
	return cfd;
}

int main(int argc, char *argv[])
{
	const char *servaddr = "127.0.0.1";
	ushort servport = 6889;
	int cfd;
	char buf[8192];
	int bufsize = sizeof(buf);
	int opmode = OPMODE_ECHO;
	int retval;

	if (argc < 3) {
		fprintf(stderr, "Usage: $0 <servaddr> <servport> [opmode]\n");
		exit(1);
	}

	servaddr = argv[1];
	servport = atoi(argv[2]);

	if (argc > 1) {
		if (strcasecmp(argv[3], "discard") == 0)
			opmode = OPMODE_DISCARD;
		else if (strcasecmp(argv[3], "echo") == 0)
			opmode = OPMODE_ECHO;
		else if (strcasecmp(argv[3], "-") == 0)
			opmode = OPMODE_READ_STDIN;
		else
			printf("Unknown opmode `%s', assume `echo'.\n", argv[1]);
	}

	cfd = connect_udp_server(servaddr, servport);
	if (cfd == -1)
		exit(1);

	for (;;)
	{
		if (opmode == OPMODE_DISCARD) {
			retval = send(cfd, buf, retval, MSG_NOSIGNAL);
			if (retval == -1) {
				perror("send");
				exit(1);
			}
		}
		else if (opmode == OPMODE_ECHO) {
			retval = send(cfd, buf, retval, MSG_NOSIGNAL);
			if (retval == -1) {
				perror("send");
				exit(1);
			}

			retval = recv(cfd, buf, bufsize, 0);
			if (retval == 0) {
				printf("??? the peer has performed an orderly shutdown\n");
				continue;
			}
			if (retval == -1) {
				perror("recv");
				exit(1);
			}

			buf[retval < bufsize? retval : bufsize - 1] = '\0';
			printf("%s", buf);
		}
		else if (opmode == OPMODE_READ_STDIN) {
			retval = read(STDIN_FILENO, buf, bufsize);
			if (retval == 0)
				exit(0);
			if (retval == -1) {
				perror("read");
				exit(1);
			}

			retval = send(cfd, buf, retval, MSG_NOSIGNAL);
			if (retval == -1) {
				perror("send");
				exit(1);
			}
		}
	}

	exit(0);
}
