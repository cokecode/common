#include "shortconn.h"

int create_udp_server(const char *addr, uint16_t port)
{
	int sfd;
	struct sockaddr_in sa;

	memset(&sa, 0, sizeof(sa));
	sa.sin_port = htons(port);
	sa.sin_family = AF_INET;
	if (addr != NULL)
		inet_pton(AF_INET, addr, &sa.sin_addr);
	else
		sa.sin_addr.s_addr = INADDR_ANY;

	if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return sfd;
	}

	if (bind(sfd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		perror("bind");
		return -1;
	}

	return sfd;
}

int connect_udp_server(const char *addr, uint16_t port, uint16_t sport)
{
	int sfd;
	struct sockaddr_in sa, sa2;

	memset(&sa, 0, sizeof(sa));
	sa.sin_port = htons(port);
	sa.sin_family = AF_INET;
	inet_pton(AF_INET, addr, &sa.sin_addr);

	if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return sfd;
	}

	memset(&sa2, 0, sizeof(sa2));
	sa2.sin_family = AF_INET;
	sa2.sin_port = htons(sport);
	sa2.sin_addr.s_addr = INADDR_ANY;

	if (bind(sfd, (struct sockaddr *)&sa2, sizeof(sa2)) < 0) {
		perror("bind");
		close(sfd), sfd = -1;
		return sfd;
	}

	if (connect(sfd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		perror("connect");
		return -1;
	}

	return sfd;
}

int create_tcp_server(const char *addr, uint16_t port)
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

/*
 * @addr - server address
 * @port - server port
 * @sport - local bind port
 */
int connect_tcp_server(const char *addr, uint16_t port, uint16_t sport)
{
	int cfd;
	struct sockaddr_in sa, sa2;
	int retval;
	int optval;

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

	optval = 1;
	setsockopt(cfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	memset(&sa2, 0, sizeof(sa2));
	sa2.sin_family = AF_INET;
	sa2.sin_port = htons(sport);
	sa2.sin_addr.s_addr = INADDR_ANY;

	if ((retval = bind(cfd, (struct sockaddr *)&sa2, sizeof(sa2))) < 0) {
		perror("bind");
		close(cfd), cfd = -1;
		return cfd;
	}

	if ((retval = connect(cfd, (struct sockaddr *)&sa, sizeof(sa))) < 0) {
		perror("connect");
		close(cfd), cfd = -1;
		return cfd;
	}

	return cfd;
}

double getsystime()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return (double)tv.tv_sec * 1000 + (double)tv.tv_usec / 1000;
}

/*
 vim:ts=4:sw=4:ai:cindent:tw=78:noet
*/
