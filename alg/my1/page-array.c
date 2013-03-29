#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <assert.h>

#define PAGE_SIZE 32

#define GUARD_VAL 0x12345678

struct mydata {
	int val;
};

struct pq_page {
	void * buckets[PAGE_SIZE];
	int _guard;
	int idx_begin, idx_end;
	struct pq_page * prev, * next;
};

struct pq_queue
{
	struct pq_page * head;
	struct pq_page * tail;
};

#define PQ_QUEUE_INIT		{NULL, NULL}

void pq_insert(struct pq_queue * pq, struct mydata * d)
{
	struct pq_page * page, * lastpage, * new;
	void ** buckets = NULL;
	int idx;

	if (!pq->head) {
		new = (struct pq_page *)malloc(sizeof(*new));
		memset(new->buckets, 0, sizeof(new->buckets));
		new->_guard = GUARD_VAL;
		new->buckets[0] = d;
		new->idx_begin = 0;
		new->idx_end = 1;
		new->prev = new->next = NULL;

		pq->head = new;
		// printf("++ page(h) %p\n", new);
		return;
	}

	for (page = pq->head; page != NULL; page = page->next) {
		// assert(page->idx_end > page->idx_begin);

		lastpage = page;
		buckets = page->buckets;

		// <1> the page is full
		if (page->idx_begin == 0 && page->idx_end == PAGE_SIZE) {
			// assert(page->idx_begin == 0);

			if (d->val >= ((struct mydata *)buckets[PAGE_SIZE - 1])->val)
				continue;

			if (d->val <= ((struct mydata *)buckets[0])->val) {
				// insert a new page
				new = (struct pq_page *)malloc(sizeof(*new));
				memset(new->buckets, 0, sizeof(new->buckets));
				new->_guard = GUARD_VAL;
				new->buckets[0] = d;
				new->idx_begin = 0;
				new->idx_end = 1;
				new->prev = page->prev;
				if (new->prev)
					new->prev->next = new;
				new->next = page;
				page->prev = new;
				if (pq->head == page)
					pq->head = new;
				// printf("++ page(i) %p\n", new);
				return;
			}

			// divide page into two

			for (idx = 0; idx != page->idx_end; idx++) {
				if (d->val < ((struct mydata *)buckets[idx])->val)
					break;
			}

			new = (struct pq_page *)malloc(sizeof(*new));
			memset(new->buckets, 0, sizeof(new->buckets));
			new->_guard = GUARD_VAL;

			memcpy(new->buckets, buckets, sizeof(void *) * idx);
				// assert(new->_guard == GUARD_VAL);
				// assert(page->_guard == GUARD_VAL);
			// printf("--> (%p) %d\n", new, sizeof(void *) * idx);
			memset(buckets, 0, sizeof(void *) * idx);
			new->buckets[idx] = d;

			new->idx_begin = 0;
			new->idx_end = idx + 1;
				// assert(new->idx_end < 33);
			page->idx_begin = idx;
			
			new->prev = page->prev;
			if (new->prev)
				new->prev->next = new;
			new->next = page;
			page->prev = new;
			if (pq->head == page)
				pq->head = new;
			// printf("++ page(d) %p\n", new);

			return;
		}

		// <2> page is not full, just put the newcomer into an empty bucket
		if (d->val >= ((struct mydata *)buckets[page->idx_end - 1])->val) {
			struct pq_page * next = page->next;
			if (next && d->val > ((struct mydata *)next->buckets[next->idx_begin])->val)
				continue;

			if (page->idx_end == PAGE_SIZE) {
				memmove(buckets + page->idx_begin - 1,
						buckets + page->idx_begin,
						sizeof(void *) * (page->idx_end - page->idx_begin));
				buckets[page->idx_end - 1] = d;
				page->idx_begin--;
			}
			else {
				buckets[page->idx_end++] = d;
				// assert(page->idx_end < 33);
			}
		}
		else {
			for (idx = page->idx_begin; idx != page->idx_end; idx++) {
				if (d->val < ((struct mydata *)buckets[idx])->val)
					break;
			}

			if (idx == page->idx_begin && idx != 0) {
				page->idx_begin--;
				buckets[page->idx_begin] = d;
			}
			else if (page->idx_end == PAGE_SIZE) {
				/* move backward */
				memmove(buckets + page->idx_begin - 1,
						buckets + page->idx_begin,
						sizeof(void *) * (idx - page->idx_begin));
				buckets[idx - 1] = d;
				page->idx_begin--;
			}
			else {
				/* move forward */
				memmove(buckets + idx + 1, buckets + idx,
						sizeof(void *) * (page->idx_end - idx));
					// assert(page->_guard == GUARD_VAL);
				// printf("==> (%p) %d\n", page, sizeof(void *) * (page->idx_end - idx));
				buckets[idx] = d;
				page->idx_end++;
					// assert(page->idx_end < 33);
			}
		}

		return;
	}

	// Not inserted yet... create a new page and append it
	new = (struct pq_page *)malloc(sizeof(*new));
	memset(new->buckets, 0, sizeof(new->buckets));
	new->_guard = GUARD_VAL;

	new->buckets[0] = d;
	new->idx_begin = 0;
	new->idx_end = 1;
	lastpage->next = new;
	new->prev = lastpage;
	new->next = NULL;
	// printf("++ page(a) %p\n", new);
}

struct mydata * pq_shift(struct pq_queue * pq)
{
	struct pq_page * page;
	struct mydata * d;

	if (!pq || !pq->head)
		return NULL;

	page = pq->head;
	assert(page->idx_end > page->idx_begin);

	d = page->buckets[page->idx_begin];
	page->buckets[page->idx_begin] = NULL;
	page->idx_begin++;

	if (page->idx_begin == page->idx_end) {
		pq->head = page->next;
		if (pq->head)
			pq->head->prev = NULL;
		free(page), page = NULL;
	}

	return d;
}

void pq_destroy(struct pq_queue * pq)
{
	struct pq_page * page, * tmp;

	if (pq) {
		page = pq->head;
		while (page) {
			tmp = page->next;
			free(page), page = NULL;
			page = tmp;
		}

		pq->head = NULL;
	}
}

void pq_dump(struct pq_queue * pq)
{
	struct pq_page * page, * tmp;
	int idx;
	int count, npage = 0, total = 0;

	if (pq) {
		page = pq->head;
		while (page) {
			npage++;

			count = 0;
			for (idx = page->idx_begin; idx != page->idx_end; idx++)
				count++;

			total += count;

			printf("%p - [%d] ", page, count);
			for (idx = page->idx_begin; idx != page->idx_end; idx++) {
				printf("%d ", ((struct mydata *)page->buckets[idx])->val);
			}
			printf("\n");

			tmp = page->next;
			free(page), page = NULL;
			page = tmp;
		}

		pq->head = NULL;
	}

	printf("npage = %d, total = %d, avg = %.2f\n",
			npage, total, (double)total / npage);
}

int main(int argc, char * argv[])
{
	struct pq_queue pq = PQ_QUEUE_INIT;
	struct mydata * d;
	int i, max, loop, c;

	while ((c = getopt(argc, argv, "m:n:")) != -1) {
		switch (c)
		{
			case 'm':
				max = atoi(optarg);
				break;
			case 'n':
				loop = atoi(optarg);
				break;
			default:
				break;
		}
	}

	printf("loop = %d\n", loop);
	printf("max = %d\n", max);

	// srand(time(NULL));
	
	while (loop-- > 0) {
		for (i = 0; i < max; i++) {
			d = (struct mydata *)malloc(sizeof(*d));
			d->val = rand() % 10000;
			pq_insert(&pq, d);
		}

		pq_dump(&pq);

		while ((d = pq_shift(&pq)) != NULL) {
			free(d), d = NULL;
		}
	}
	
	return 0;
}
