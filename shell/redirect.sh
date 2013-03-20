#! /bin/bash


exec >log.stdout 2>log.stderr
echo info
echo >&2 error

exec </etc/passwd
exec >log.stdout2 2>log.stderr2
cat
