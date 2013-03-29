#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <assert.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>

#include "LoadavgCollector.h"

LoadavgCollector::LoadavgCollector()
{
	m_softload = 0;
	m_loadavg = 0;
	m_running = 0;
	m_tid = 0;
}

LoadavgCollector::~LoadavgCollector()
{
	assert(!m_running);
}

int LoadavgCollector::__read_sysload()
{
	FILE * fp;
	float load1, load5, load15;

	fp = fopen("/proc/loadavg", "r");
	fscanf(fp, "%f %f %f", &load1, &load5, &load15);
	fclose(fp), fp = NULL;

	return (int)(load1 * 100);
}

int LoadavgCollector::__calc_loadavg()
{
	int sysload = __read_sysload();

	return sysload > m_softload? sysload : m_softload;
}

void * LoadavgCollector::__proc(void * arg)
{
	LoadavgCollector * obj = (LoadavgCollector *)arg;

	printf("==> thread start\n");

	while (obj->m_running) {
		obj->m_loadavg = obj->__calc_loadavg();

		for (int i = 0; i < 5 && obj->m_running; i++)
			sleep(1);
	}

	printf("==> thread stop\n");
	return NULL;
}

int LoadavgCollector::Start()
{
	int retval;

	m_running = 1;
	retval = pthread_create(&m_tid, NULL, __proc, this);
	if (retval < 0)
		m_running = 0;

	return retval;
}

int LoadavgCollector::Stop()
{
	m_running = 0;
	return 0;
}

int LoadavgCollector::Wait()
{
	if (m_tid)
		pthread_join(m_tid, NULL), m_tid = 0;

	return 0;
}
