#! /bin/bash

cmdline="$0 $@"
opt_background=yes
opt_indaemon=

if [ "$1" == "--indaemon" ]; then
	opt_indaemon=yes
fi

if [ "$opt_background" == "yes" -a "$opt_indaemon" == "" ]; then
    cmd="setsid $cmdline --indaemon &"   ## Note setsid: more check?
    echo "command: [$cmd]"
    eval "$cmd" || exit 255 
    echo "INFO: daemon OK."
    exit 0
fi

while [ 1 ]; do
	sleep 1
done

