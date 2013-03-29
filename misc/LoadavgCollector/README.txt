
LoadavgCollector.cpp
LoadavgCollector.h

    LoadavgCollector类定义和实现。

Makefile
Makefile.template

    Makefile。

app-cpu-bound.pl
app-io-bound.pl

    模拟系统负载的脚本。app-cpu-bound.pl偏重于计算，app-io-bound.pl偏重于IO操作。

test.cpp
test-loadavg

    前者是测试程序代码，编译后生成后者。它展示和测试Collector类的使用，运行的时候
    可以同时运行上述脚本，或者其它能产生系统负载的程序，以观察效果。

