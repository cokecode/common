#! /bin/bash
# cokecoder AT gmail.com

# calculate sum of rss (resident set size) all processes.
#ps ax -o pid,rss,cmd | sort -nk 2 | awk '{print $2}' | awk '{sum+=$1}END{print sum}'
ps ax -o rss | awk '{sum+=$1}END{print sum}'
