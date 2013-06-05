#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

void convert(const char * s, int reverse, int hex)
{
	unsigned long n;
	char * a;
	char * endp = NULL;

	if (s == NULL || strlen(s) == 0)
		return;

	if (reverse) {
		/* number -> ascii */
		n = strtoul(s, &endp, 0);

		if (strlen(s) != (endp - s))
			printf("Invalid number '%s'\n", s);
		else {
			a = inet_ntoa(*((struct in_addr *)&n));
			printf("%s\n", a);
		}
	}
	else {
		/* ascii -> number */
		n = inet_addr(s);
		if (hex)
			printf("0x%08lx\n", n);
		else
			printf("%lu\n", n);
	}
}

void print_usage()
{
	printf("Utility inet-pton: convert ip address between string"
			" and integer format.\n");
	printf("Version 1.0, 2006/06/05, yongbao.chen@net263.com\n");
	printf("Usage:\n");
	printf("   inet-pton [-v] [-x] [-n] arg1 arg2 ...\n");
	printf("Example:\n");
	printf("   ./inet-pton 192.168.3.1\n");
	printf("   17017024\n");
	printf("   ./inet-pton 17017024 -v\n");
	printf("   192.168.3.1\n");
	printf("   ./inet-pton -x 192.168.3.1\n");
	printf("   0x0103a8c0\n");
	printf("   ./inet-pton -x -v 0x0103a8c0\n");
	printf("   192.168.3.1\n");
}

int main(int argc, char * argv[])
{
	int i;
	int reverse = 0;
	int hex = 0;
	int ntohl = 0;

	if (argc == 1) {
		print_usage();
		exit(0);
	}

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 ||
			strcmp(argv[i], "--help") == 0)
		{
			print_usage();
			exit(0);
		}

		if (strcmp(argv[i], "-v") == 0 ||
			strcmp(argv[i], "--reverse") == 0)
		{
			reverse = 1;
			argv[i][0] = 0;
		}

		if (strcmp(argv[i], "-x") == 0 ||
			strcmp(argv[i], "--hex") == 0)
		{
			hex = 1;
			argv[i][0] = 0;
		}

		if (strcmp(argv[i], "-n") == 0 ||
			strcmp(argv[i], "--ntohl") == 0)
		{
			ntohl = 1;
			argv[i][0] = 0;
		}
	}

	if (ntohl) {
		for (i = 1; i < argc; i++) {
			if (strlen(argv[i]) > 0) {
				if (hex)
					printf("0x%08lx\n", htonl(strtoul(argv[i], NULL, 0)));
				else
					printf("%lu\n", htonl(strtoul(argv[i], NULL, 0)));
			}
		}
	}
	else {
		for (i = 1; i < argc; i++) {
			convert(argv[i], reverse, hex);
		}
	}

	return 0;
}
