#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint32_t ip_compress(uint32_t ip)
{
	ip &= 0x7fffffff;       // bit 31
	ip = ((ip & (~((1 << 24) - 1))) >> 1) | (ip & ((1 << 23) - 1));
	ip = ((ip & (~((1 << 22) - 1))) >> 1) | (ip & ((1 << 21) - 1));
	ip = ((ip & (~((1 << 18) - 1))) >> 1) | (ip & ((1 << 17) - 1));
	ip >>= 1;
	return ip;
}

int main()
{
	uint32_t ip;

	ip = ip_compress(0x80a20001);
	printf("%08x\n", ip);

	ip = 1 | (1 << 17) | (1 << 21) | (1 << 23) | (1 << 31);
	printf("%08x\n", ip);
	printf("%08x\n", ip_compress(ip));
	printf("%08x\n", ~ip);
	printf("%08x\n", ip_compress(~ip));		// should be 0x07ffffff

	return 0;
}
