#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define SIZE	(512 * 1024 * 1024)

int main()
{
	struct timeval begin, now;
	double f0, f1, f2;
	double sum;
	char *buf;
	int i;

	gettimeofday(&begin, NULL);		f0 = begin.tv_sec * 1000000 + begin.tv_usec;
	printf("BEGIN:\t%lu:%lu\n", begin.tv_sec, begin.tv_usec);

	buf = malloc(SIZE);
	if (buf == NULL) {
		printf("malloc failed\n");
		exit(1);
	}
	
	gettimeofday(&now, NULL);
	f1 = now.tv_sec * 1000000 + now.tv_usec;
	printf("NOW:\t%lu:%lu\n", now.tv_sec, now.tv_usec);

	for (i = 0; i < 20; i++) {
		gettimeofday(&now, NULL);
		f1 = now.tv_sec * 1000000 + now.tv_usec;

		memset(buf, 0, SIZE);

		gettimeofday(&now, NULL);
		f2 = now.tv_sec * 1000000 + now.tv_usec;

		printf("memset time:	%.2f microsec\n", f2 - f1);
		sum += f2 - f1;
	}

	printf("avg. memset time: %.2f microsec\n", sum / 20);

	free(buf);
}
