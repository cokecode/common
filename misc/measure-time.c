#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>

void measure_time(struct timeval * last)
{
	struct timeval now;
	int ret;

	if ((ret = gettimeofday(&now, NULL)) < 0) {
		perror("gettimeofday failed");
		return;
	}

	if (last == NULL || last->tv_sec == 0)
		printf("(%ld.%06ld)\n", now.tv_sec, now.tv_usec);
	else {
		int32_t a, b;
		a = now.tv_sec - last->tv_sec;
		b = now.tv_usec - last->tv_usec;
		if (b < 0) {
			a--;
			b += 1000000;
		}
		printf("(%ld.%06ld) %d.%06d\n", now.tv_sec, now.tv_usec, a, b);
	}

	*last = now;
}

int main()
{
	struct timeval tv = {0, 0};

	measure_time(&tv);

	sleep(1);

	measure_time(&tv);

	sleep(2);

	measure_time(&tv);

	return 0;
}
