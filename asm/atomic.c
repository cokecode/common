#include <stdio.h>
#include "atomic.h"

#define PRINT_VALUE(msg) \
	printf(#msg": a = %d\n", atomic_read(&a));

int main()
{
	atomic_t a = ATOMIC_INIT(9);
	int c;

	PRINT_VALUE("initial");

	atomic_add(4, &a);
		PRINT_VALUE("add 4");
	atomic_sub(5, &a);
		PRINT_VALUE("sub 5");
	atomic_inc(&a);
		PRINT_VALUE("inc");
	atomic_dec(&a);
		PRINT_VALUE("dec");

	c = atomic_sub_and_test(8, &a);
		printf("sub and test 10: %d, %d\n", atomic_read(&a), c);
	c = atomic_inc_and_test(&a);
		printf("inc and test: %d, %d\n", atomic_read(&a), c);
	c = atomic_dec_and_test(&a);
		printf("dec and test: %d, %d\n", atomic_read(&a), c);

	return 0;
}
