#! /bin/bash

#lftp -u pubftp,look 172.24.52.147 <<EOF
lftp -u pubftp,look pubftp.tbsite.net <<EOF
cd /
$*
bye
EOF
