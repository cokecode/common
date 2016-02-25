#! /bin/bash

if [ $# -eq 0 -o "$1" = "--help" -o "$1" = "-h" ]; then
	echo "Usage: $0 qword filename-pattern"
	echo "e.g.,"
	echo "   $0 exit '*.[c|h]??'"
	echo "   $0 exit '*.py'"
	exit 1
fi

progname=`basename $0`
qword="$1"
filenamepattern="$2"

case "$progname" in
	"grep.x") grepopt="--color -HRwn";;
	"grep.xx") grepopt="--color -HRwni";;
	"grep.xxx") grepopt="--color -HRn";;
	"grep.xxxx") grepopt="--color -HRni";;
	*) "progname should be one of grep.x / grep.xx / grep.xxx"; exit 1;;
esac

if [ -z "$filenamepattern" ]; then
	find -type f -print0 | xargs --null -n200 grep $grepopt "$qword"
else
	if [ "${filenamepattern#*.}" = "${filenamepattern}" ]; then
		filenamepattern="*.$filenamepattern"
	fi
	find -name "$filenamepattern" -print0 | xargs --null -n200 grep $grepopt "$qword"
fi
