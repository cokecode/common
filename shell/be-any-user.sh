#! /bin/bash
## become any user

if [ $# -eq 0 ]; then
	echo "Usage: $0 user"
	exit 1
fi

user=$1
opt=$2
cmd=$0

set -x
if [ "$opt" == "" ]; then
	exec sudo $cmd $user --be-root
elif [ "$opt" == "--be-root" ]; then
	exec sudo bash -c "$cmd $user --be-user"
elif [ "$opt" == "--be-user" ]; then
	exec su - $user
fi
