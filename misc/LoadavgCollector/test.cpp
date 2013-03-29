#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "LoadavgCollector.h"

LoadavgCollector c;

void sighdl(int signum)
{
	c.Stop();
}

int main(int argc, char * argv[])
{
	signal(SIGINT, sighdl);
	signal(SIGTERM, sighdl);
	signal(SIGQUIT, sighdl);

	c.Start();

	while (c.IsRunning()) {
		sleep(5);
		c.UpdateSoftload(rand() % 100);
		printf("sysload = %d\n", c.GetLoadavg());
	}

	c.Wait();

	return 0;
}
