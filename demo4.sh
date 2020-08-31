#!/bin/dash

a=0

while [ $a -gt 1 ]
do
    echo $a #comment same line as echo
    a=`expr $a + 1`
done
