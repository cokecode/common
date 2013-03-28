#include <stdio.h>
#include <signal.h>

void sighdl(int signum)
{
	printf("Ctrl-C pressed\n");
}

int main()
{
	signal(SIGINT, sighdl);

	while (1)
		sleep(999);
}
