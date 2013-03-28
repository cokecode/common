/*
 * utime - wrapper of utime(2)
 * chenyb@yztech.com.cn
 * 2008-04-07
 */

/*
 * gcc -Wall -o utime utime.c
 */

#define _XOPEN_SOURCE			// for strptime

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <utime.h>
#include <errno.h>

int main(int argc, char * argv[])
{
	const char * filename;
	const char * timestr;
	struct tm tm;
	struct utimbuf ut;

	if (argc != 3) {
		printf("Usage: %s <filename> <\"YYYY-mm-dd HH:MM:SS\">\n", argv[0]);
		exit(1);
	}

	filename = argv[1];
	timestr = argv[2];

	if (strptime(timestr, "%F %T", &tm) == NULL) {
		printf("Invalid time format: [%s]\n", timestr);
		exit(1);
	}

	ut.actime = 0;
	ut.modtime = mktime(&tm);
	if (utime(filename, &ut) < 0) {
		printf("utime() failed: errno = %d\n", errno);
		exit(1);
	}

	return 0;
}

