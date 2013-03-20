#! /bin/bash

cmdline="$0 $@"
while [ $# -gt 0 ]; do
    case $1 in
        -b|--background) opt_background=yes;;
        --indaemon) opt_indaemon=yes;;
        *) usage;;
    esac
    shift
done

if ! exec >>/root/abc.txt; then
    echo "redirect failed, quit"
    exit 255
fi
exit

if [ "$opt_background" == "yes" -a "$opt_indaemon" == "" ]; then
    sleep 0.3   ## if something run, this make the forking slower
    cmd="setsid $cmdline --indaemon &"
    echo "$cmd"
    eval "$cmd"
    exit 0
fi

if [ "$opt_background" == "yes" ]; then
    echo "run in background"
    exec </dev/null ## this is not necessary, but make things clear
    exec >/dev/null 2>&1
    sleep 999
else
    echo "run in foreground"
fi
