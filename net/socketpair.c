#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

int main()
{
	int fd[2];
	int retval;
	char buf[1024 * 64 - 512];

	retval = socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
	if (retval == -1) {
		perror("socketpair");
		exit(1);
	}

	retval = write(fd[0], buf, sizeof(buf));
	printf("wrote %d bytes\n", retval);
	retval = read(fd[1], buf, sizeof(buf));
	printf("read %d bytes\n", retval);

	retval = write(fd[1], buf, sizeof(buf));
	printf("wrote %d bytes\n", retval);
	retval = read(fd[0], buf, sizeof(buf));
	printf("read %d bytes\n", retval);

	close(fd[0]);
	close(fd[1]);

	exit(0);
}
