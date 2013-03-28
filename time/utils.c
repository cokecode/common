#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

long floortime_minute(long time)
{
	return (long)(floor(time / 60.0)) * 60;
}

long floortime_hour(long time)
{
	return (long)(floor(time / 3600.0)) * 3600;
}

long floortime_day(long time)
{
	return (long)(floor(time / 86400.0)) * 86400;
}

long ceiltime_minute(long time)
{
	return (long)(ceil(time / 60.0)) * 60;
}

long ceiltime_hour(long time)
{
	return (long)(ceil(time / 3600.0)) * 3600;
}

long ceiltime_day(long time)
{
	return (long)(ceil(time / 86400.0)) * 86400;
}

int main(int argc, char * argv[])
{
	printf("%ld\n", floortime_minute(time(NULL)));
	printf("%ld\n", floortime_hour(time(NULL)));
	printf("%ld\n", floortime_day(time(NULL)));
	printf("%ld\n", ceiltime_minute(time(NULL)));
	printf("%ld\n", ceiltime_hour(time(NULL)));
	printf("%ld\n", ceiltime_day(time(NULL)));
	printf("round: %d\n", (int)roundf(atof(argv[1])));
	printf("nearbyint: %d\n", (int)nearbyintf(atof(argv[1])));
}
