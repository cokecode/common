#include <stdio.h>

#define offset_of(type, member) (&((type *)0)->member - 0)
#define container_of(type, member, ptr) \
    ((type *)((const char *)ptr - (int)offset_of(type, member)))



typedef struct computer computer;
struct computer
{
	int cpu;
	int mem;
	int kb;
	int mouse;
};

#define PR_INFO(fmt, x...) \
	do { \
		printf("[INFO] "); \
		printf(fmt, ##x); \
		printf("\n"); \
	} while (0)

int main()
{
	computer comp, * pcomp;
	int * pmem = &comp.mem;
	int * pcpu = &comp.cpu;
	int * pkb  = &comp.kb;
	int * pmouse = &comp.mouse;

	printf("computer  = %p\n", &comp);
	printf("x1        = %p\n", container_of(computer, cpu, pcpu));
	printf("x2        = %p\n", container_of(computer, mem, pmem));
	printf("x3        = %p\n", container_of(computer, kb, pkb));
	printf("x4        = %p\n", container_of(computer, mouse, pmouse));
	PR_INFO("hello, world!");
	PR_INFO("hello, %s! (error: %d)", "world", 12345);
}
