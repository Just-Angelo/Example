#!/bin/bash
# DESCRIPTION:
# This is an university assignment and is basically a watered down version of GIT. The four commands are shrug-init, shrug-add, shrug-commit and shrug-log.
# The code has certain assumptions like a files name not having a space.
# How each works:
# shrug-init: simply creates a repository if it doesn't already exist. used in the form of ./shrug-init.sh .
# shrug-add: In the form of ./shrug-add.sh file1 file2 ...... and adds a file in the current directory to the "index", an area where files are added to before being commited.
# shrug-commit: In the form of ./shrug-commit.sh [-a] -m message(no spaces for message), commits files from the index to the repository with a message in log. If -a(optional) is selected it will
# update current files in the index with those in the current directory.
# shrug-log: In the form of ./shrug-log.sh and simply shows the messages of past commits.
name=".shrug"
Arg_count=$#
#checks no additonal user input
if [ $Arg_count != 0 ]
then
	echo "usage: shrug-init"
	exit
fi

#My code works by creating different numbers of repositry to hold different 
#stages of commit, with repos0 the first.

if [ -d "$name" ]
then
	echo "shrug-init: error: .shrug already exists"
else
	echo "Initialized empty shrug repository in .shrug"
	mkdir -p .shrug/index
    	mkdir -p .shrug/repos0
    	touch .shrug/log
fi
