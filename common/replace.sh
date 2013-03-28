#! /bin/bash

oldword=zstring
newword=string

for f in $(find -name '*.h' -o -name '*.cpp'); do
    mv $f ${f}.orig2
    perl -pe "s/\b$oldword\b/$newword/g" < ${f}.orig2 > $f
done
