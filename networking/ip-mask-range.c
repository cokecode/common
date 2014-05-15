// cokecoder@gmail.com, 2014.5
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void usage()
{
	printf("Usage:\n");
	printf("   ./ip-mask-range xx.xx.xx.xx/xx\n");
}

// Usage:
//   ./ip-mask-range 192.168.37.0/22
//   will output
//     192.168.36.0 - 192.168.39.255
//     192.168.36.0/22
//     192.168.36.0/255.255.252.0
int main(int argc, char * argv[])
{
	char * input;
	char * sep;		// position of "/"
	const char * ipstr;
	const char * maskstr;
	char range_begin[256];
	char range_end[256];
	char tmp[255];
	uint32_t ip;
	uint32_t network;
	uint32_t subnet_max;
	int mask;

	if (argc < 2) {
		usage();
		exit(1);
	}

	input = argv[1];

	// parse & seperate
	sep = strchr(input, '/');
	if (sep == NULL) {
		usage();
		exit(2);
	}

	ipstr = input;
	maskstr = sep + 1;
	*sep = '\0';

	// check ip & mask
	if (!inet_pton(AF_INET, ipstr, &ip)) {
		printf("Invalid ip address.\n");
		exit(3);
	}
	ip = ntohl(ip);

	mask = atoi(maskstr);
	if (mask < 0 || mask > 32) {
		printf("Invalid mask (should be: 0 < mask < 32)\n");
		exit(4);
	}

	network = ip & (~((1 << (32 - mask)) - 1));
	subnet_max = (1 << (32 - mask)) - 1;

	ip = htonl(network + 0);
	inet_ntop(AF_INET, &ip, range_begin, sizeof(range_begin));
	ip = htonl(network + subnet_max);
	inet_ntop(AF_INET, &ip, range_end, sizeof(range_end));

	printf("%s - %s\n", range_begin, range_end);
	printf("%s/%d\n", range_begin, mask);
	ip = htonl(~((1 << (32 - mask)) - 1));
	inet_ntop(AF_INET, &ip, tmp, sizeof(tmp));
	printf("%s/%s\n", range_begin, tmp);
	return 0;
}
