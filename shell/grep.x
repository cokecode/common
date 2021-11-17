#! /bin/bash
# A powerful tool for find + grep. Before using it, do a simple setup:
#
#   ln -s grep.x grep.xw
#   ln -s grep.x grep.xi
#   ln -s grep.x grep.xiw
#   ln -s grep.x grep.xwi
#
# examples,
#
#   grep.xw import py       # search in py files, case sensitive, match whole word
#   grep.xiw imporT java    # search in java files, case insensitive, match whole word
#   grep.x mport sh         # search in *.sh files, case insensitive, match partly word
#   grep.x qword            # search all file
#
# zhangxun@taobao.com
# 2014/06
# 2021/11

if [ $# -eq 0 -o "$1" = "--help" -o "$1" = "-h" ]; then
	echo "Usage: $0 qword filename-pattern"
	echo "e.g.,"
	echo "   $0 qword '*.[c|h]*'"
	echo "   $0 qword py"
	echo "   $0 qword '*.[c|j|p]*"
	exit 1
fi

progname=`basename $0`
qword=$1
filenamepattern="$2"

case "$progname" in
	"grep.x") grepopt="--color -Hn";;
	"grep.xw") grepopt="--color -Hnw";;
	"grep.xi") grepopt="--color -Hni";;
	"grep.xiw") grepopt="--color -Hniw";;
	"grep.xwi") grepopt="--color -Hniw";;
	*) echo >&2 "Incorrect command name. see the script for details."; exit 1;;
esac

if [ -z "$filenamepattern" ]; then
	find -type f | grep -v -e '/.git/' -e '/.svn/' | xargs -n200 grep $grepopt $qword
else
	# e.g., convert 'py' to '*.py', but if keep it unchanged if it already is '*.py'.
	if [ "${filenamepattern#*.}" = "${filenamepattern}" ]; then
		filenamepattern="*.$filenamepattern"
	fi
	find -type f -name "$filenamepattern" | grep -v -e '/.git/' -e '/.svn/' | xargs -n200 grep $grepopt $qword
fi

# vim:ts=4:sw=4:ai:et:ft=sh
