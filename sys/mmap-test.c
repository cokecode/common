// Test mmap & munmap.
// When two processes mmap the same file, `ps' show every process has big RSS.
// Wed Jan  9 17:15:09 CST 2013
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define DATAFILE	"data1.txt"

int main()
{
	int fd;
	unsigned char *buf;
	struct stat st;
	unsigned long filesize;
	int retval;
	int tmp = 0;
	int i = 0;

	fd = open(DATAFILE, O_RDONLY, 0);
	if (fd < 0) {
		perror("failed to open file");
		exit(1);
	}

	memset((void *)&st, 0, sizeof(st));
	if (fstat(fd, &st) < 0) {
		perror("stat failed");
		exit(1);
	}
	filesize = st.st_size;

	printf("open file <%s> ok, size = %lu\n", DATAFILE, filesize);

	buf = (unsigned char *)mmap(NULL, filesize, PROT_READ, MAP_SHARED, fd, 0);
	if (buf == NULL) {
		perror("failed to mmap: %s\n");
		exit(1);
	}
	close(fd), fd = -1;
	printf("mmap ok, buf = %p. closed fd.\n", buf);

	printf("sleep 180s (you may kill sleep) ...\n");
	system("sleep 180");

	printf("reading buf ...\n");
	while (i < filesize) {
		tmp += buf[i];
		i++;
	}
	printf("tmp = %d\n", tmp);
	printf("sleep 180s (you may kill sleep) ...\n");
	system("sleep 180");

	retval = munmap(buf, filesize);
	printf("munmap, retval = %d\n", retval);

	printf("sleep 180s (you may kill sleep) ...\n");
	system("sleep 180");

	return 0;
}

