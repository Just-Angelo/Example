#!/bin/bash

name=$1
Arg_count=$#
# Makes sure argument number are valid.
if [ $Arg_count -le 0 ]
then
    	echo "usage: shrug-add <filenames>"
   	exit;
fi 
#checks .shrug actually exist
if [ ! -d ".shrug" ]
then
    	echo "shrug-add: error: no .shrug directory containing shrug repository exists"
    	exit;
fi
       
for args in $*
do  
    	counter_dir=0
    	counter_index=0
    #loops through working directory and if theres a match with arg adds to index.
    	for files in *
    	do
        	if [ "$args" = "$files" ]
        	then
            #counter_dir keeps track if a document exists in the working directory.
            		counter_dir=$((counter_dir+1))  
            		cp $files .shrug/index      
        	fi                        
    	done
    
    	for docs in .shrug/index/*
    	do 
        	doc_cut=$(echo $docs | cut -f3 -d "/") 
        	if [ "$args" = "$doc_cut" ]
        	then
            #If a folder inside the index matches the user input but no longer 
            #exists in the working directory, we remove it.
            		if [ $counter_dir -eq 0 ]
            		then
                		rm -r $docs
            		fi
            		counter_index=$((counter_index+1))       
        	fi   
    	done
    #If an argument doesn't exist in both the index and working directory then
    #we print the following error.
    	if [ $counter_dir -eq 0 -a $counter_index -eq 0 ]
    	then
        	echo "shrug-add: error: can not open 'non_existent_file'"
    	fi
done
