#! /bin/bash

if [ $# -ne 1 ]; then
    echo "This script generate all IPs from IP range file."
    echo "Usage: $0 idname"
    echo "Example,"
    echo "   $0 CN"
    echo "   $0 HK"
    exit 1
fi

idname=$1

if [ ! -d "IPDATA-$idname" ]; then
    echo "ERROR: directory [ IPDATA-$idname ] not exists."
    exit 1
fi

cd IPDATA-$idname || exit 255

../gen-allip-from-range < iprange-merged.txt

