#include <stdio.h>

class A {};
class B : public A {};
void foo(A * & a) { a = new B; }
int main()
{
	B * b/* = 0*/;     // 这里必须初始化，否则带优化选项时编译器有警告
	// foo((A *)b);
	// foo(reinterpret_cast<A *&>(b));
	foo((A *&)b);
	printf("b = %p\n", b);
	// foo返回时，b仍然是NULL，与我们想象的相违背。
}
