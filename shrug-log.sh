#!/bin/bash
repos_num=-1
Arg_count=$#
#checks .shrug actually exist
if [ ! -d ".shrug" ]
then
    	echo "shrug-add: error: no .shrug directory containing shrug repository exists"
    	exit;
fi
if [ $Arg_count != 0 ]
then
    	echo "usage: shrug-log"
    	exit
fi
#counts how many stages of commit there is, as theres an inital file repos_num
#is initialized as -1.
for files in .shrug/repos*
do
	repos_num=$((repos_num+1))
done
if [ $repos_num = 0 ]
then
	echo "shrug-log: error: your repository does not have any commits yet"
    	exit
fi 
cat .shrug/log
