#!/bin/dash
for stuff in *.c
do
    read line   #comment in same line as non-comment
    echo $stuff $line #comment in same line as non-comment
done
