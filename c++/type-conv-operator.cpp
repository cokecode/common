#include <stdio.h>
#include <string.h>

class A
{
public:
	A() {
		strcpy(m_data, "hello, world!");
	}

	operator const char *()
	{
		return m_data;
	}

private:
	char m_data[1024];
};

void foo(const char * str)
{
	printf("a = <%s>\n", str);
}

int main()
{
	A a;
	foo(a);
}
