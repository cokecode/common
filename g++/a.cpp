#include <stdio.h>

class A {};
class B : public A
{
	int b;
};

void foo(A * &p)
{
	p = new B;
}

int main()
{
	A * a;
	B * b = 0;

	foo(a);
	foo((A *)b);

	printf("a = %p, b = %p\n", a, b);
}
