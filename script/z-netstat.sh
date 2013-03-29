#! /bin/bash

while sleep 10; do
    date
    for port in 6541 6543 6544 6545 4000 5000; do
        netstat -natp | grep $port
    done
done
