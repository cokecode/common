Class LoadavgCollector README
-------------------------------------------------

�ඨ��
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


��Ա˵��
========

    LoadavgCollector();
    ~LoadavgCollector();

        ���������������û������֮����

    int Start();

        ����Collector�̣߳�����0��ʾ�ɹ���-1��ʾʧ�ܡ�ʧ�ܵ�ԭ����errno��ʶ��

    int Stop();

        ֹͣCollector�̡߳����Ƿ���0��ʾ�ɹ���

    int IsRunning();

        ��ѯCollector�߳��Ƿ������У����� 1 ��ʾ�����У�0 ��ʾ�������С�

    int Wait();

        ���Collector�߳������У���ȴ�������������ʲôҲ���������Ƿ���0��

    void UpdateSoftload(int val);

        ���á����ء�ֵ�����ر�ʾӦ�ó����Լ��Ķ���ĸ�����������涨����
        �ı仯��Χ�ʹ�С��ϵͬ����ָ����
        Ӧ�ò�ģ��Ӧ�����Եص��ô˺������Ը���Collector���µĸ���״����
        ���Ӧ�ò�ģ��û�����صĸ��������������������

    int GetLoadavg();

        ���ص�ǰ�ĸ���ָ��������ָ���ķ�Χ�� [0, 10000] ��0 ��ʾϵͳ�������ᣬ
        10000 ��ʾϵͳ�������ء�

��˵��
======

    �������Collector����֮�󣬾Ϳ��Ե���Start()�������������߳��ˡ�Collector
    �߳������ԣ�ÿ��5���ӣ��̶�ֵ�����ռ�ϵͳ������Ϣ��Ӧ�ò�ͨ������Update-
    Softload()����������Collector��ǰ��������Ϣ��

    ����Stop()����������ֹCollector�̵߳����С�



