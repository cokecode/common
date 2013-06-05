/*
 * Following code of this file can only be compiled on i386 platform.
 */

#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#ifdef CONFIG_SMP

#define LOCK_PREFIX \
                ".section .smp_locks,\"a\"\n"   \
                "  .align 4\n"                  \
                "  .long 661f\n" /* address */  \
                ".previous\n"                   \
                "661:\n\tlock; "
#else // ! CONFIG_SMP

#define LOCK_PREFIX
				
#endif

#define ATOMIC_INIT(i)		{ (i) }
#define atomic_read(v)		((v)->counter)
#define atomic_set(v,i)		(((v)->counter) = (i))

typedef struct { volatile int counter; } atomic_t;

/**
 * atomic_add - add integer to atomic variable
 * @i: integer value to add
 * @v: pointer of type atomic_t
 * 
 * Atomically adds @i to @v.
 */
static __inline__ void atomic_add(int i, atomic_t *v)
{
	__asm__ __volatile__(
			LOCK_PREFIX "addl %1,%0"
			:"=m" (v->counter)
			:"ir" (i), "m" (v->counter));
}

/**
 * atomic_sub - subtract the atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 * 
 * Atomically subtracts @i from @v.
 */
static __inline__ void atomic_sub(int i, atomic_t *v)
{
	__asm__ __volatile__(
			LOCK_PREFIX "subl %1,%0"
			:"=m" (v->counter)
			:"ir" (i), "m" (v->counter));
}

/**
 * atomic_sub_and_test - subtract value from variable and test result
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 * 
 * Atomically subtracts @i from @v and returns
 * true if the result is zero, or false for all
 * other cases.
 */
static __inline__ int atomic_sub_and_test(int i, atomic_t *v)
{
	unsigned char c;

	__asm__ __volatile__(
			LOCK_PREFIX "subl %2,%0; sete %1"
			:"=m" (v->counter), "=qm" (c)
			:"ir" (i), "m" (v->counter) : "memory");
	return c;
}

/**
 * atomic_inc - increment atomic variable
 * @v: pointer of type atomic_t
 * 
 * Atomically increments @v by 1.
 */ 
static __inline__ void atomic_inc(atomic_t *v)
{
	__asm__ __volatile__(
			LOCK_PREFIX "incl %0"
			:"=m" (v->counter)
			:"m" (v->counter));
}

/**
 * atomic_dec - decrement atomic variable
 * @v: pointer of type atomic_t
 * 
 * Atomically decrements @v by 1.
 */ 
static __inline__ void atomic_dec(atomic_t *v)
{
	__asm__ __volatile__(
			LOCK_PREFIX "decl %0"
			:"=m" (v->counter)
			:"m" (v->counter));
}

/**
 * atomic_dec_and_test - decrement and test
 * @v: pointer of type atomic_t
 * 
 * Atomically decrements @v by 1 and
 * returns true if the result is 0, or false for all other
 * cases.
 */ 
static __inline__ int atomic_dec_and_test(atomic_t *v)
{
	unsigned char c;

	__asm__ __volatile__(
			LOCK_PREFIX "decl %0; sete %1"
			:"=m" (v->counter), "=qm" (c)
			:"m" (v->counter) : "memory");
	return c != 0;
}

/**
 * atomic_inc_and_test - increment and test 
 * @v: pointer of type atomic_t
 * 
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */ 
static __inline__ int atomic_inc_and_test(atomic_t *v)
{
	unsigned char c;

	__asm__ __volatile__(
			LOCK_PREFIX "incl %0; sete %1"
			:"=m" (v->counter), "=qm" (c)
			:"m" (v->counter) : "memory");
	return c != 0;
}

/**
 * atomic_add_negative - add and test if negative
 * @v: pointer of type atomic_t
 * @i: integer value to add
 * 
 * Atomically adds @i to @v and returns true
 * if the result is negative, or false when
 * result is greater than or equal to zero.
 */ 
static __inline__ int atomic_add_negative(int i, atomic_t *v)
{
	unsigned char c;

	__asm__ __volatile__(
			LOCK_PREFIX "addl %2,%0; sets %1"
			:"=m" (v->counter), "=qm" (c)
			:"ir" (i), "m" (v->counter) : "memory");
	return c;
}

#endif // !__ATOMIC_H__
