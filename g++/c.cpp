class A {};
class B : public A {};
void foo(A * & a) { a = new B; }
int main()
{
	B * b/* = 0*/;     // ��������ʼ����������Ż�ѡ��ʱ�������о���
	foo((A *)b);
	// foo����ʱ��b��Ȼ��NULL���������������Υ����
}
