#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include "atomic.h"

atomic_t g_counter;

void * proc(void * arg)
{
	for (;;) {
		void * p = malloc(256);
		free(p);
		atomic_inc(&g_counter);
	}
}

int main(int argc, char * argv[])
{
	int i, n;
	atomic_set(&g_counter, 0);

	n = argc > 1? atoi(argv[1]) : 6;

	for (i = 0; i < n; i++) {
		pthread_t tid;
		pthread_create(&tid, NULL, proc, NULL);
	}

	sleep(10);
	printf("counter = %d\n", atomic_read(&g_counter));

	return 0;
}
