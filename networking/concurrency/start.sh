#! /bin/bash

TOTAL=1
SERVADDR=192.168.31.222
SERVPORT=6889

if [ $# -gt 0 ]; then
    TOTAL=$1
fi

echo $TOTAL

for ((i=1; $i<=$TOTAL; i=$i+1)); do
    ./tcp_client $SERVADDR $SERVPORT &
    ((x=$i%50))
    if [ $x -eq 0 ]; then
        sleep 5
    fi
done
