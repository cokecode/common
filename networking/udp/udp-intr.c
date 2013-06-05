#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <assert.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

enum {
	MYP_NONE,
	MYP_STEP1,
	MYP_STEP2,
	MYP_STEP3,
	MYP_STEP4,
};

typedef unsigned char uchar;

void usage()
{
	printf(
	"Usage:\n"
	"   udp-intr -s [ options ] [ port ]\n"
	"   udp-intr -c [ options ] host [ port ]\n"
	"default port is 6889.\n"
	"Options:\n"
	"   -c --client           client mode\n"
	"   -s --server           server mode\n"
	"   -p --pkgsize ##       send/recv package size\n"
	"   -n --nr-session ##    number of transaction\n"
	"   -w --nr-worker ##     number of worker process (client only)\n"
	);
}

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

ssize_t xsend(int s, const void *msg, size_t len, int flags)
{
	ssize_t retval = send(s, msg, len, flags);
	if (retval == -1) {
		perror("send");
		exit(1);
	}
	else {
		// printf("send %d bytes\n", retval);
	}

	return retval;
}

ssize_t xrecv(int s, void *buf, size_t len, int flags)
{
	ssize_t retval = recv(s, buf, len, flags);
	if (retval == -1) {
		perror("recv");
		exit(1);
	}
	else {
		// printf("recv %d bytes\n", retval);
	}

	return retval;
}

ssize_t xsendto(int s, const void *msg, size_t len, int flags,
		const struct sockaddr *to, socklen_t tolen)
{
	ssize_t retval = sendto(s, msg, len, flags, to, tolen);
	if (retval == -1) {
		perror("sendto");
		exit(1);
	}
	else {
		// printf("send %d bytes\n", retval);
	}

	return retval;
}

ssize_t xrecvfrom(int s, void *buf, size_t len, int flags,
		struct sockaddr *from, socklen_t *fromlen)
{
	ssize_t retval = recvfrom(s, buf, len, flags, from, fromlen);
	if (retval == -1) {
		perror("recvfrom");
		exit(1);
	}
	else {
		// printf("recv %d\n", retval);
	}

	return retval;
}

double xgettime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (double)tv.tv_sec * 1000 + (double)tv.tv_usec / 1000;
}

int main(int argc, char * argv[])
{
	const char * host = NULL;
	ushort port = 6889;
	int serv_mode = 1;
	char buf[8192];
	int pkgsize = 0;
	int nsession = 0;
	int nworker = 0;

	struct option opts[] = {
		{ "help", 0, NULL, 'h' },
		{ "version", 0, NULL, 'v' },
		{ "client", 0, NULL, 's' },
		{ "server", 0, NULL, 'c' },
		{ "pkgsize", 1, NULL, 'p' },
		{ "nr-session", 1, NULL, 'n' },
		{ "nr-worker", 1, NULL, 'w' },
		{ NULL, 0, NULL, 0 },
	};
	int c;

	while ((c = getopt_long(argc, argv, "hvcsp:n:w:", opts, NULL)) != -1)
	{
		switch (c)
		{
			case 'h':
			case 'v':
				usage();
				exit(1);
			case 'c':
				serv_mode = 0;
				break;
			case 's':
				serv_mode = 1;
				break;
			case 'p':
				pkgsize = atoi(optarg);
				break;
			case 'n':
				nsession = atoi(optarg);
				break;
			case 'w':
				nworker = atoi(optarg);
				break;
			default:
				exit(1);
		}
	}

	if (serv_mode) {
		if (optind + 1 <= argc)
			port = atoi(argv[optind]);
	}
	else {
		if (optind + 1 <= argc)
			host = argv[optind];
		if (optind + 2 <= argc)
			port = atoi(argv[optind + 1]);
	}

	if (pkgsize == 0)
		pkgsize = 1024;
	if (pkgsize > (1500 - 20 - 8))
		pkgsize = (1500 - 20 - 8);

	if (nsession == 0)
		nsession = 10000;

	if (nworker == 0)
		nworker = 1;

	printf("serv_mode = %d\n", serv_mode);
	printf("host = %s\n", host);
	printf("port = %d\n", port);
	printf("pkgsize = %d\n", pkgsize);
	printf("nsession = %d\n", nsession);
	printf("nworker = %d\n", nworker);

	if (serv_mode) {
		int sfd;
		struct sockaddr from;
		socklen_t socklen;
		int nrecv;
		int k;

		if ((sfd = create_udp_server(NULL, port)) == -1)
			exit(1);

		printf("udp server started, sockfd = %d\n", sfd);

		for (k = 0; k < nworker; k++) {
			if (fork() == 0) {
				for (;;) {
					socklen = sizeof(from);

					nrecv = xrecvfrom(sfd, buf, sizeof(buf), 0, &from, &socklen);
					if (buf[0] == MYP_STEP4)
						continue;

					{
						int i; double prod;
						for (i = 0; i < nrecv; i++) {
							prod *= buf[i] + sqrt(buf[i]);
						}

						memcpy(buf, &prod, sizeof(prod));
					}

					xsendto(sfd, buf, pkgsize, 0, &from, socklen);
				}
			}
		}
	}
	else {
		int cfd;
		int i, k;
		double begin, end;

		for (k = 0; k < nworker; k++) {
			if (fork() == 0) {
				if ((cfd = connect_udp_server(host, port)) == -1)
					exit(1);

				printf("connected, sockfd = %d\n", cfd);

				begin = xgettime();

				for (i = 0; i < nsession; i++) {
					buf[0] = MYP_STEP1;
					xsend(cfd, buf, pkgsize, 0);
					xrecv(cfd, buf, sizeof(buf), 0);

					buf[0] = MYP_STEP2;
					xsend(cfd, buf, pkgsize, 0);
					xrecv(cfd, buf, sizeof(buf), 0);

					buf[0] = MYP_STEP3;
					xsend(cfd, buf, pkgsize, 0);
					xrecv(cfd, buf, sizeof(buf), 0);

					buf[0] = MYP_STEP4;
					xsend(cfd, buf, pkgsize, 0);
				}

				end = xgettime();

				printf("count = %d, time = %g, avg = %.2f\n",
						nsession, (end - begin) / 1000,
						nsession / (end - begin) * 1000);

				exit(0);
			}
		}
	}

	while (wait(NULL) > 0);
	return 0;
}
