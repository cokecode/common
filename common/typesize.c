#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define PSIZE(x)	printf(#x" = %lu\n", x)

int main()
{
	PSIZE(sizeof(char));
	PSIZE(sizeof(short));
	PSIZE(sizeof(int));
	PSIZE(sizeof(long));
	PSIZE(sizeof(long long));
	PSIZE(sizeof(int16_t));
	PSIZE(sizeof(int32_t));
	PSIZE(sizeof(int64_t));
	PSIZE(sizeof(size_t));
	PSIZE(sizeof(ssize_t));
	PSIZE(sizeof(float));
	PSIZE(sizeof(double));
	PSIZE(sizeof(long double));
	PSIZE(sizeof(time_t));
	return 0;
}
