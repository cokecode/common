/*
 * Test resource usage by mutex / inter-thread communication.
 */

#include <pthread.h>

int max = 10 * 1000 * 1000;
pthread_mutex_t mutex;
pthread_cond_t cond_inc;
pthread_cond_t cond_dec;
int val;

void * thr(void * arg)
{
	int i;

	for (i = 0; i < max; i++) {
		pthread_mutex_lock(&mutex);
		pthread_mutex_unlock(&mutex);
	}
}

void * thr_inc(void * arg)
{
	int i;

	pthread_mutex_lock(&mutex);
	for (i = 0; i < max - 1; i++) {
		if (val > 0) {
			/* ... */
			val = -1;
			pthread_cond_signal(&cond_dec);
		}
		else if (val == 0)
			break;
		pthread_cond_wait(&cond_inc, &mutex);
	}
	val = 0;
	pthread_cond_signal(&cond_dec);
	pthread_mutex_unlock(&mutex);
}

void * thr_dec(void * arg)
{
	int i;
	pthread_mutex_lock(&mutex);
	for (i = 0; i < max; i++) {
		if (val < 0) {
			/* ... */
			val = 1;
			pthread_cond_signal(&cond_inc);
		}
		else if (val == 0)
			break;
		pthread_cond_wait(&cond_dec, &mutex);
	}
	val = 0;
	pthread_cond_signal(&cond_inc);
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char * argv[])
{
	pthread_t tid1, tid2;
	int level = 0;
	int i;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_inc, NULL);
	pthread_cond_init(&cond_dec, NULL);

	if (argc > 1) {
		level = atoi(argv[1]);
	}

	if (level == 0) {
		thr(&mutex);
		thr(&mutex);
	}
	if (level == 1) {
		pthread_create(&tid1, NULL, thr, NULL);
		pthread_create(&tid2, NULL, thr, NULL);
		pthread_join(tid1, NULL);
		pthread_join(tid2, NULL);
	}
	if (level == 2) {
		val = 1;
		max = 1000 * 1000;
		pthread_create(&tid1, NULL, thr_inc, NULL);
		pthread_create(&tid2, NULL, thr_dec, NULL);
		pthread_join(tid2, NULL);
		pthread_join(tid1, NULL);
	}
}
