/*
 * addr.c, yongbao.chen@net263.com
 * 2006-05-23
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef WIN32
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <arpa/inet.h>
#endif

void print_error(int errcode)
{
	switch (errcode)
	{
		case EAI_FAMILY:
			fprintf(stderr, "%s - %s\n", "EAI_FAMILY", gai_strerror(EAI_FAMILY));
			break;
		case EAI_SOCKTYPE:
			fprintf(stderr, "%s - %s\n", "EAI_SOCKTYPE", gai_strerror(EAI_SOCKTYPE));
			break;
		case EAI_BADFLAGS:
			fprintf(stderr, "%s - %s\n", "EAI_BADFLAGS", gai_strerror(EAI_BADFLAGS));
			break;
		case EAI_NONAME:
			fprintf(stderr, "%s - %s\n", "EAI_NONAME", gai_strerror(EAI_NONAME));
			break;
		case EAI_SERVICE:
			fprintf(stderr, "%s - %s\n", "EAI_SERVICE", gai_strerror(EAI_SERVICE));
			break;
		case EAI_ADDRFAMILY:
			fprintf(stderr, "%s - %s\n", "EAI_ADDRFAMILY", gai_strerror(EAI_ADDRFAMILY));
			break;
		case EAI_NODATA:
			fprintf(stderr, "%s - %s\n", "EAI_NODATA", gai_strerror(EAI_NODATA));
			break;
		case EAI_MEMORY:
			fprintf(stderr, "%s - %s\n", "EAI_MEMORY", gai_strerror(EAI_MEMORY));
			break;
		case EAI_FAIL:
			fprintf(stderr, "%s - %s\n", "EAI_FAIL", gai_strerror(EAI_FAIL));
			break;
		case EAI_AGAIN:
			fprintf(stderr, "%s - %s\n", "EAI_AGAIN", gai_strerror(EAI_AGAIN));
			break;
		case EAI_SYSTEM:
			perror("getaddrinfo");
			break;
		default:
			assert(0);
			break;
	}
}

void dump_addrinfo(struct addrinfo * ai)
{
	do {
		printf("dump ai ==>\n");
		printf("    ai_flags = %08x\n", ai->ai_flags);
		printf("   ai_family = %d\n", ai->ai_family);
		printf(" ai_socktype = %d\n", ai->ai_socktype);
		printf(" ai_protocol = %d\n", ai->ai_protocol);
		printf("  ai_addrlen = %d\n", ai->ai_addrlen);
		printf("ai_canonname = '%s'\n", ai->ai_canonname);

		if (ai->ai_family == AF_INET) {
			struct sockaddr_in * sin;
			sin = (struct sockaddr_in *)ai->ai_addr;

			printf("    sockaddr = %s:%d\n",
					inet_ntoa(sin->sin_addr),
					ntohs(sin->sin_port));
		}

		ai = ai->ai_next;
	} while (ai);
}

void foo1()
{
	struct addrinfo hint, * ai = NULL;
	int retval;

	printf("==> %s\n", __FUNCTION__);

	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = 0;
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;

	retval = getaddrinfo(NULL, "http", &hint, &ai);
	if (retval != 0) {
		print_error(retval);
		exit(1);
	}

	printf("retval = %d\n", retval);
	dump_addrinfo(ai);
	freeaddrinfo(ai), ai = NULL;
}

void foo2()
{
	struct addrinfo hint, * ai = NULL;
	int retval;

	printf("==> %s\n", __FUNCTION__);

	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = AI_PASSIVE;
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;

	retval = getaddrinfo(NULL, "http", &hint, &ai);
	if (retval != 0) {
		print_error(retval);
		exit(1);
	}

	printf("retval = %d\n", retval);
	dump_addrinfo(ai);
	freeaddrinfo(ai), ai = NULL;
}

void foo3()
{
	struct addrinfo hint, * ai = NULL;
	int retval;

	printf("==> %s\n", __FUNCTION__);

	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = 0;
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;

	retval = getaddrinfo("192.168.200.1", "http", &hint, &ai);
	if (retval != 0) {
		print_error(retval);
		exit(1);
	}

	printf("retval = %d\n", retval);
	dump_addrinfo(ai);
	freeaddrinfo(ai), ai = NULL;
}

void foo4()
{
	struct addrinfo hint, * ai = NULL;
	int retval;

	printf("==> %s\n", __FUNCTION__);

	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = 0;
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;

	retval = getaddrinfo("192.168.200.1", NULL, &hint, &ai);
	if (retval != 0) {
		print_error(retval);
		exit(1);
	}

	printf("retval = %d\n", retval);
	dump_addrinfo(ai);
	freeaddrinfo(ai), ai = NULL;
}

void foo5()
{
	struct addrinfo hint, * ai = NULL;
	int retval;

	printf("==> %s\n", __FUNCTION__);

	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = 0;
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;

	retval = getaddrinfo("localhost.localdomain", NULL, &hint, &ai);
	if (retval != 0) {
		print_error(retval);
		exit(1);
	}

	printf("retval = %d\n", retval);
	dump_addrinfo(ai);
	freeaddrinfo(ai), ai = NULL;
}

int main()
{
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif

	foo1();
	foo2();
	foo3();
	foo4();
	foo5();

	return 0;
}
