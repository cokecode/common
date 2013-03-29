Class LoadavgCollector README
-------------------------------------------------

类定义
======

class LoadavgCollector
{
public:
    LoadavgCollector();
    ~LoadavgCollector();

    int Start();
    int Stop();
    int IsRunning();
    int Wait();

    void UpdateSoftload(int val);
    int GetLoadavg();

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


成员说明
========

    LoadavgCollector();
    ~LoadavgCollector();

        构造和析构函数，没有特殊之处。

    int Start();

        启动Collector线程，返回0表示成功，-1表示失败。失败的原因由errno标识。

    int Stop();

        停止Collector线程。总是返回0表示成功。

    int IsRunning();

        查询Collector线程是否在运行，返回 1 表示在运行，0 表示不在运行。

    int Wait();

        如果Collector线程在运行，则等待它结束；否则什么也不做。总是返回0。

    void UpdateSoftload(int val);

        设置“软负载”值。软负载表示应用程序自己的定义的负载量，这里规定软负载
        的变化范围和大小关系同负载指数。
        应用层模块应周期性地调用此函数，以告诉Collector最新的负载状况。
        如果应用层模块没有软负载的概念，则无需调用这个函数。

    int GetLoadavg();

        返回当前的负载指数。负载指数的范围是 [0, 10000] ，0 表示系统负载最轻，
        10000 表示系统负载最重。

类说明
======

    构造完成Collector对象之后，就可以调用Start()函数启动它的线程了。Collector
    线程周期性（每隔5秒钟，固定值）地收集系统负载信息。应用层通过调用Update-
    Softload()函数来告诉Collector当前的软负载信息。

    调用Stop()函数可以终止Collector线程的运行。



