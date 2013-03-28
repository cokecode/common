#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
	int stat;
	int i;

	for (i = 1; i < argc; i++) {
		stat = atoi(argv[i]);
		printf("exit status = %d:\n", stat);
		if (WIFEXITED(stat))
			printf("WIFEXITED, %d\n", WEXITSTATUS(stat));
		if (WIFSIGNALED(stat))
			printf("WIFSIGNALED, %d\n", WTERMSIG(stat));
		if (WIFSTOPPED(stat))
			printf("WIFSTOPPED, %d\n", WSTOPSIG(stat));
		if (WIFCONTINUED(stat))
			printf("WIFCONTINUED\n");
	}
}
