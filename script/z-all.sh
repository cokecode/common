#! /bin/bash

./z-ps.sh > ps.log &
./z-vmstat.sh > vmstat.log &
./z-netstat.sh > netstat.log &
./z-sar.sh > sar.log &
./z-iostat.sh > iostat.log &
