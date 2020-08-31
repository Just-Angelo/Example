#!/bin/bash

option=$1
message=$2
index_count=0
match_count=0
commit_count=$(wc -l < .shrug/log )
repos_num=-1
arg_num=$#
repos_count=0

#checks .shrug actually exist
if [ ! -d ".shrug" ]
then
	echo "shrug-add: error: no .shrug directory containing shrug repository exists"
    	exit;
fi
#checks theres atleast 2 arguments.
if [ $arg_num -le 1 ]
then
    	echo "usage: shrug-commit [-a] -m commit-message"
    	exit
fi
#counts how many stages of commit there is, as theres an inital file repos_num
#is initialized as -1.
for files in .shrug/repos*
do
    	repos_num=$((repos_num+1))
done
#counts how many files exist in the index.
for files in .shrug/index/*
do
    	if [ "$files" != ".shrug/index/*" ]
    	then
        	index_count=$((index_count+1))
    	fi
done
#counts how many files there is in the current commit
for files in .shrug/repos$repos_num/*
do
    	if [ "$files" != ".shrug/repos$repos_num/*" ]
    	then
        	repos_count=$((repos_count+1))
    	fi
done
#if -a is the first argument we loop through the working directory and make sure
#to update files with identical names as ones in the index.
if [ "$option" = "-a" ]
then
    	for files in *
    	do 
        	for docs in .shrug/index/*
        	do
            		file_cut=$(echo $files | cut -f3 -d "/")
            		doc_cut=$(echo $docs | cut -f3 -d "/") 
        
            		if [ "$file_cut" = "$doc_cut" ] 
            		then
                		cp $files .shrug/index
            		fi
        	done 
    	done
fi
#counts how many files in index are identical to a file in the current commit.
for files in .shrug/index/*
do     
    	for docs in .shrug/repos$repos_num/*
    	do 
        	file_cut=$(echo $files | cut -f3 -d "/")
        	doc_cut=$(echo $docs | cut -f3 -d "/") 
        	line=$(cmp --silent $files $docs || echo "different")       
        	if [ "$file_cut" = "$doc_cut" -a "$line" != "different" ] 
        	then
            		match_count=$((match_count+1))
        	fi
    	done
done
#if the amount of files that are identical are equal to total files of both index
#and repositry then there is nothing to do.
if [ "$match_count" = "$index_count" -a "$match_count" = "$repos_count" ]
then
    	echo "nothing to commit"
    	exit;
fi

if [ "$option" = "-m" ]
then
    #checks correct number of arguments
    	if [ $arg_num != 2 ]
    	then
        	echo "usage: shrug-commit [-a] -m commit-message"
        	exit
    	fi
    #A new repositry folder for our new commit.
    	mkdir -p .shrug/repos$((repos_num+1))
    #throws everything in the index to our new commit.
    	if [ $index_count != 0 ]
    	then
        	cp .shrug/index/* .shrug/repos$((repos_num+1))
    	fi
    #checks if log isn't empty then we use sed otherwise we just us >>.   
    	if [ "$commit_count" -ge 1 ]
    	then
        	sed -i "1s/^/$commit_count $message\n/" .shrug/log
    	else
        	echo "$commit_count $message" >> .shrug/log
    	fi
    	echo "Committed as commit $commit_count"
   
elif [ "$option" = "-a" ]
then
    #Checks some basic argument requirements.
    	if [ $arg_num != 3 -o $2 != "-m" ]
    	then
        	echo "usage: shrug-commit [-a] -m commit-message"
        	exit
    	fi
    	message=$3
    #A new repositry file for our new commit.
    	mkdir -p .shrug/repos$((repos_num+1))
    #Copies everything in index to new commit.   
    	if [ $index_count != 0 ]
    	then
        	cp .shrug/index/* .shrug/repos$((repos_num+1))
    	fi
    #uses sed if its not empty otherwise uses >>.  
    	if [ "$commit_count" -ge 1 ]
    	then
        	sed -i "1s/^/$commit_count $message\n/" .shrug/log
    	else
        	echo "$commit_count $message" >> .shrug/log
    	fi
    	echo "Committed as commit $commit_count"       
	else
    		echo "usage: shrug-commit [-a] -m commit-message"
fi

