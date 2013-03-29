#! /bin/bash

echo > xxx.tmp
for prog in TitanCMDS TitanVTDU TitanNRU cmus; do
    pgrep $prog >> xxx.tmp
done

PLIST=`cat xxx.tmp`
echo $PLIST
rm -f xxx.tmp

while sleep 3; do
    ps -o ppid,pid,stat,uid,start,time,pcpu,cpu,cstime,cutime,rss,vsize,cmd $PLIST
done
