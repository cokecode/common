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

#define CHECKERROR(retval, msg) \
	if ((retval) == -1) { \
		perror((msg)); \
		exit(1); \
	}
	
int main()
{
	int cfd;
	int retval;
	int optval;
	socklen_t optlen;

	cfd = socket(AF_INET, SOCK_DGRAM, 0);
	CHECKERROR(cfd, "socket");

	retval = getsockopt(cfd, SOL_SOCKET, SO_SNDBUF, &optval, &optlen);
	CHECKERROR(retval, "getsockopt");
	printf("SO_SNDBUF = %d\n", optval);

	retval = getsockopt(cfd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
	CHECKERROR(retval, "getsockopt");
	printf("SO_RCVBUF = %d\n", optval);

	exit(0);
}
