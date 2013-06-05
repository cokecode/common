// Simple TCP client.
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
	OPMODE_DISCARD,			// Write garbage data to server and never read
	OPMODE_ECHO,			// Write garbage data to server and read
	OPMODE_READ_STDIN,		// Read from stdin and write to server, never read
							// (like ftp upload)
};

int connect_tcp_server(const char *addr, ushort port)
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

	cfd = socket(AF_INET, SOCK_STREAM, 0);
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

// Usage: $0 <servaddr> <servport> [opmode]
int main(int argc, char *argv[])
{
	const char *servaddr = "127.0.0.1";
	ushort servport = 6889;
	int cfd = 0;
	char buf[8192];
	int bufsize = sizeof(buf);
	int retval;
	int opmode = OPMODE_ECHO;

	if (argc < 3) {
		fprintf(stderr, "Usage: $0 <servaddr> <servport> [opmode]\n");
		exit(1);
	}

	servaddr = argv[1];
	servport = atoi(argv[2]);

	if (argc > 3) {
		if (strcmp(argv[3], "discard") == 0)
			opmode = OPMODE_DISCARD;
		else if (strcmp(argv[3], "echo") == 0)
			opmode = OPMODE_ECHO;
		else if (strcmp(argv[3], "-") == 0)
			opmode = OPMODE_READ_STDIN;
		else
			printf("Unknown opmode `%s', assume `echo'.\n", argv[1]);
	}

	cfd = connect_tcp_server(servaddr, servport);
	if (cfd == -1)
		exit(1);

	printf("connected\n");

	for (;;)
	{
		if (opmode == OPMODE_ECHO)
		{
			retval = write(cfd, buf, bufsize);
			if (retval == -1) {
				perror("write");
				exit(1);
			}
			if (retval < bufsize) {
				printf("oops, wrote less bytes(%d/%d)...\n", retval, bufsize);
			}

			retval = read(cfd, buf, bufsize);
			if (retval == 0) {
				printf("disconnected\n");
				exit(0);
			}
			if (retval == -1) {
				perror("read");
				exit(1);
			}
		}
		else if (opmode == OPMODE_DISCARD)
		{
			retval = write(cfd, buf, bufsize);
			if (retval == -1) {
				perror("write");
				exit(1);
			}
			if (retval < bufsize) {
				printf("oops, wrote less bytes(%d/%d)...\n", retval, bufsize);
			}
		}
		else if (opmode == OPMODE_READ_STDIN)
		{
			static int bytes_counter = 0;

			retval = read(STDIN_FILENO, buf, bufsize);
			if (retval == 0) {
				printf("done.\n");
				exit(0);
			}
			if (retval == -1) {
				perror("read");
				exit(1);
			}

			retval = write(cfd, buf, retval);
			if (retval == -1) {
				perror("write");
				exit(1);
			}

			bytes_counter += retval;
			// printf("wrote %d bytes\n", bytes_counter);
		}
	}

	exit(0);
}
