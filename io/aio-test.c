// This program is for test read disk in Asynchronous IO + direct IO.
// CopyRight (c) 2008 Seals.  All Rights Reserved.
// chenyb@yztech.com.cn
// 2008-04

#include <libaio.h>
#include <aio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <sys/time.h>

// const int READING_RATE = 1024 * 1024;
int g_rate = -1;

struct running_context;
struct aio_block;

typedef void (*handle_open_fxn)(struct aio_block *ab);
typedef void (*handle_read_fxn)(struct aio_block *ab, struct iocb *iocb, long res, long res2);
typedef void (*handle_timeout_fxn)(struct aio_block *ab);

struct aio_block {
	struct running_context * rc;
	struct iocb __iocb;
	handle_open_fxn handle_open;
	handle_read_fxn handle_read;
	handle_timeout_fxn handle_timeout;

	char filename[256];
	int fd;
	char * buf;
	size_t size;
	int64_t begin_time;
	int timer_key;
	double rate_array[1024];
	int rate_index;
};

struct timer_event_slot {
	struct aio_block * aioblock;
	int64_t abs_time;
};

struct timer_queue {
	int64_t abs_timeout;
	struct timer_event_slot slots[1024];
	int nr;
};

struct running_context {
	io_context_t aioctx;
	struct timer_queue * timerq;
	int64_t now;					// current system time in millisecond

	// don't use these members directly
	struct timer_queue __timerq;
};

// return time in millisecond
int64_t getsystime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (int64_t)tv.tv_sec * 1000 + (int64_t)tv.tv_usec / 1000;
}

int timer_queue_add(struct timer_queue *q, struct aio_block *ab, int64_t abs_future)
{
	int i;

	for (i = 0; i < sizeof(q->slots)/sizeof(q->slots[0]); i++) {
		if (q->slots[i].abs_time == 0) {
			q->slots[i].aioblock = ab;
			q->slots[i].abs_time = abs_future;
			if (q->abs_timeout == 0 || abs_future < q->abs_timeout)
				q->abs_timeout = abs_future;
			q->nr++;
			// printf("tq add: %s, key=%d, nr=%d\n", ab->filename, i, q->nr);
			return i;
		}
	}

	abort();
	return -1;
}

// int timer_queue_del(struct timer_queue * q, int key)
int timer_queue_del(struct aio_block *ab, int key)
{
	struct timer_queue *q = ab->rc->timerq;
	assert(key >= 0 && key < sizeof(q->slots)/sizeof(q->slots[0]));
	assert(q->slots[key].aioblock == ab);
	
	// printf("tq del: %s, key=%d, nr=%d\n", q->slots[key].aioblock->filename, key, q->nr - 1);
	q->slots[key].abs_time = 0;
	q->slots[key].aioblock = NULL;
	q->nr--;

	if (q->nr == 0)
		q->abs_timeout = 0;
	else {
		int i;
		for (i = 0; i < sizeof(q->slots)/sizeof(q->slots[0]); i++) {
			if (q->slots[i].abs_time > 0 && q->slots[i].abs_time < q->abs_timeout)
				q->abs_timeout = q->slots[i].abs_time;
		}
	}
	return 0;
}

int timer_queue_update(struct timer_queue * q, int key, int64_t abs_future)
{
	assert(key >= 0 && key < sizeof(q->slots)/sizeof(q->slots[0]));

	q->slots[key].abs_time = abs_future;
	if (abs_future < q->abs_timeout)
		q->abs_timeout = abs_future;
	return 0;
}

void init_running_context(struct running_context * rc, io_context_t aioctx)
{
	rc->aioctx = aioctx;
	rc->timerq = &rc->__timerq;
	rc->now = 0;
	memset(&rc->__timerq, 0, sizeof(rc->__timerq));
}

int foo(io_context_t aioctx, struct aio_block * abs, int nr)
{
	struct running_context rctx;
	struct timer_queue *timerq;
	struct iocb ** iocbs;
	struct io_event aioev[1024];
	struct timespec * timeout, timespec;
	int retval;
	int i;

	init_running_context(&rctx, aioctx);
	iocbs = (struct iocb **)malloc(sizeof(struct iocb *) * nr);
	memset(iocbs, 0, sizeof(iocbs));
	timerq = rctx.timerq;

	rctx.now = getsystime();

	// initial submittings
	for (i = 0; i < nr; i++) {
		struct aio_block * ab = &abs[i];
		iocbs[i] = &ab->__iocb;
		io_prep_pread(iocbs[i], ab->fd, ab->buf, ab->size, 0);
		io_set_callback(iocbs[i], (io_callback_t)ab);
		ab->rc = &rctx;
		ab->handle_open(ab);
		ab->timer_key = -1;
	}
	if ((retval = io_submit(aioctx, nr, iocbs)) < 0) {
		errno = -retval;
		perror("io_submit");
		exit(1);
	}

	printf("initial io_submit(): submitted %d request\n", retval);

	while (1) {
		if (timerq->abs_timeout == 0)
			timeout = NULL;
		else {
			// printf("timer: %lld, now: %lld\n", timerq->abs_timeout, rctx.now);
			timespec.tv_sec = (timerq->abs_timeout - rctx.now) / 1000;
			timespec.tv_nsec = ((timerq->abs_timeout - rctx.now) % 1000) * 1000000;
			// printf("x: timeout:%lld, %ld, %ld\n", timerq->abs_timeout, timespec.tv_sec, timespec.tv_nsec);
			if (timespec.tv_sec < 0)
				timespec.tv_sec = 0;
			if (timespec.tv_nsec < 0)
				timespec.tv_nsec = 0;
			if (timespec.tv_sec == 0 && timespec.tv_nsec == 0) {
				timespec.tv_sec = 0;
				timespec.tv_nsec = 1;
			}
			timeout = &timespec;
			// printf("P: timeout:%lld, %ld, %ld\n", timerq->abs_timeout, timespec.tv_sec, timespec.tv_nsec);
			// printf("P: timerq->nr = %d\n", timerq->nr);
		}
		
		// printf("io_getevents ...\n");
		memset(aioev, 0, sizeof(aioev));
		retval = io_getevents(aioctx, 1, sizeof(aioev) / sizeof(aioev[0]), aioev, timeout);
		if (retval < 0) {
			perror("io_getevents");
			exit(1);
		}

		rctx.now = getsystime();

		// printf("ok, io_getevents(): %d\n", retval);

		// dispatch timer events
		for (i = 0, nr = 0; nr < timerq->nr; i++) {
			// printf("Gtimer: %lld, Gnow: %lld\n", timerq->slots[i].abs_time, rctx.now);
			if (timerq->slots[i].abs_time != 0) {
				if (timerq->slots[i].abs_time <= rctx.now)
					timerq->slots[i].aioblock->handle_timeout(timerq->slots[i].aioblock);
				nr++;
			}
		}

		// dispatch disk-aio events
		for (i = 0; i < retval; i++) {
			struct io_event * ev = &aioev[i];
			struct aio_block * ab = (struct aio_block *)ev->data;
			ab->handle_read(ab, ev->obj, ev->res, ev->res2);
		}
	}

	free(iocbs), iocbs = NULL;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////

// return:
// 	1: don't read any more
// 	0: continue
int ratectrl(int64_t begin_time, int64_t now_time, int64_t bytes)
{
	int64_t rate;
	if (now_time == begin_time)
		return 0;
	rate = bytes / (now_time - begin_time) * 1000 * 8;
	return rate > g_rate; 
}

void save_rate_record(struct aio_block *ab, double rate)
{
	if (ab->rate_index < sizeof(ab->rate_array) / sizeof(ab->rate_array[0]))
		ab->rate_array[ab->rate_index++] = rate;
}

void display_rate_record(struct aio_block *ab)
{
	int i;
	for (i = 0; i < ab->rate_index; i++) {
		printf("%s: %.2f\n", ab->filename, ab->rate_array[i]);
	}
}

void handle_open(struct aio_block *ab)
{
	ab->begin_time = ab->rc->now;
	memset(ab->rate_array, 0, sizeof(ab->rate_array));
	ab->rate_index = 0;
}

void handle_read(struct aio_block *ab, struct iocb *iocb, long res, long res2)
{
	struct running_context *rc = ab->rc;
	int retval;

	// printf("handle_read: %s: res = %ld, res2 = %ld\n", ab->filename, res, res2);

	if (iocb->u.c.offset % (1 * 1024 * 1024) == 0) {
		double rate;
		rate = (double)iocb->u.c.offset / (rc->now - ab->begin_time);	// bytes/duration
		rate *= 1000;	// sec -> millisec
		rate *= 8;		// byte -> bit
		save_rate_record(ab, rate);

		if (random() % 100 == 0)
			printf("%s: offset=%lld, rate=%.2fb/s\n", ab->filename, iocb->u.c.offset, rate);
	}

	if (res > 0) {
		iocb->u.c.offset += res;

		if (ratectrl(ab->begin_time, rc->now, iocb->u.c.offset)) {
			ab->timer_key = timer_queue_add(rc->timerq, ab, rc->now + 50);
			return;
		}

		retval = io_submit(rc->aioctx, 1, (struct iocb **)&iocb);
		if (retval < 0) {
			errno = -retval;
			perror("xxxxxxx");
			exit(3);
		}
	}
	else {
		printf("==> %s: done\n", ab->filename);
		close(ab->fd), ab->fd = -1;
		if (ab->timer_key != -1) {
			timer_queue_del(ab, ab->timer_key);
			ab->timer_key = -1;
		}

		display_rate_record(ab);
		// TODO other ...
	}

	return;
}

void handle_timeout(struct aio_block * ab)
{
	struct running_context *rc = ab->rc;
	// printf("handle_timeout (%s, %d)\n", ab->filename, ab->timer_key);
	timer_queue_del(ab, ab->timer_key); ab->timer_key = -1;

	if (ratectrl(ab->begin_time, rc->now, ab->__iocb.u.c.offset)) {
		ab->timer_key = timer_queue_add(rc->timerq, ab, rc->now + 50);
	}
	else {
		struct iocb *iocbs[1] = { &ab->__iocb };
		if (io_submit(rc->aioctx, 1, iocbs) < 0) {
			perror("xxxxxxxyy");
			exit(3);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////

int read_file_list(const char *fname, char *filelist[], int size)
{
	FILE *fp;
	char line[1024];
	int count = 0;
	int i;

	fp = fopen(fname, "r");

	while ((fgets(line, sizeof(line), fp)) != NULL) {
		count++;
	}
	fseek(fp, 0, SEEK_SET);
	i = 0;
	while ((fgets(line, sizeof(line), fp)) != NULL) {
		if( line[strlen(line) - 1] == '\r' )
			line[strlen(line) - 1] = '\0';
		if( line[strlen(line) - 1] == '\n' )
			line[strlen(line) - 1] = '\0';
		filelist[i++] = strdup(line);
	}

	fclose(fp), fp = NULL;
	return count;
}

int main(int argc, char *argv[])
{
	char *filelist[1024];
	io_context_t aioctx = 0;
	struct aio_block *abs = NULL;
	int nr = 0;
	int retval = -1;
	int i;

	const int MAX_AIO_EVENT = 1024;
	const int AIO_BLOCK_SIZE = 512 * 1024;

	if (argc > 2) {
		nr = read_file_list(argv[1], filelist, 1024);
		g_rate = atoi(argv[2]);
	}
	else {
		printf("usage: program <filelist> <bitrate>\n");
		printf("BITRATE: 1M -> 1048576, 1.5M -> 1572864, 2M -> 2097152\n");
		exit(1);
	}

	abs = (struct aio_block *)calloc(nr, sizeof(struct aio_block));
	for (i = 0; i < nr; i++) {
		struct aio_block * ab = &abs[i];
		strncpy(ab->filename, filelist[i], sizeof(ab->filename));

		ab->fd = open(ab->filename, O_RDONLY | O_DIRECT);

		ab->size = AIO_BLOCK_SIZE;
		if (posix_memalign((void **)(&ab->buf), 4096/*alignment*/, ab->size) < 0) {
			perror("posix_memalign");
			exit(1);
		}
		memset(ab->buf, 0, ab->size);

		ab->handle_read = handle_read;
		ab->handle_timeout = handle_timeout;
		ab->handle_open = handle_open;
	}

	memset(&aioctx, 0, sizeof(aioctx));
	if ((retval = io_setup(MAX_AIO_EVENT, &aioctx)) < 0) {
		errno = -retval;
		perror("io_setup");
		exit(1);
	}

	foo(aioctx, abs, nr);

	return 0;
}
