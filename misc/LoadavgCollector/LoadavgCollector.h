#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__

#include <pthread.h>

/**
 * class LoadavgCollector: see README-Collector.txt for details.
 */
class LoadavgCollector
{
public:
	LoadavgCollector();
	~LoadavgCollector();

	int Start();
	int Stop();
	int IsRunning() { return m_running; }
	int Wait();

	void UpdateSoftload(int val) { m_softload = val; }
	int GetLoadavg() { return m_loadavg; }

	enum
	{
		LOADAVG_MIN = 0,
		LOADAVG_MAX = 10000,
	};

private:
	int __read_sysload();
	int __calc_loadavg();
	static void * __proc(void *);

private:
	volatile int m_softload;
	volatile int m_loadavg;
	volatile int m_running;
	pthread_t m_tid;
};

#endif // __COLLECTOR_H__
