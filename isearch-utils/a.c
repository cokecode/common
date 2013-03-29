#include <stdio.h>

int main()
{
	long long x = 0x12345678ABCDEF00;
	printf("x = %016llx\n", x);
	printf("x = %016lx\n", (int)x);
	printf("%d\n", sizeof(long));
}
