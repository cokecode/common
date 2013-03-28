#include <stdio.h>
#include <stdint.h>

#define PSIZE(x)	printf(#x" = %d\n", x)

int main()
{
	PSIZE(sizeof(short));
	PSIZE(sizeof(int));
	PSIZE(sizeof(long));
	PSIZE(sizeof(long long));
	PSIZE(sizeof(int16_t));
	PSIZE(sizeof(int32_t));
	PSIZE(sizeof(int64_t));
	PSIZE(sizeof(float));
	PSIZE(sizeof(double));
	PSIZE(sizeof(long double));
}
