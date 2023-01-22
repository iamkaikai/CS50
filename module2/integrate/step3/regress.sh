#!/bin/bash


dirName=$1
fileName=$2
fileName_clean=$(basename $fileName .test)
logFolder=$(date +"%Y%m%d.%H%M%S")
dirName_exist=0

echo -e "\n"

if [ $# -lt 2 ]; then
		echo '[x] need at least 2 arguments!'
		exit 1
else
		echo '[v] num of arguments >= 2!!!!!!'
fi


if [ -r $fileName ]; then
    echo "[v] The file '$fileName' is readable!"
else														
    echo "[x] The file does not exist or not readable!"
		exit 2 
fi															


if [ -d $dirName ]; then
    echo "[v] The folder '$dirName' already exists"
		dirName_exist=1
else
		if [ -f $dirName ]; then
			echo "[x] not a directory!!"
			exit 3
		else
			mkdir $dirName
		fi
fi															


#create time-stamp folder
mkdir -p $logFolder
if [ $? -ne 0 ];then
		echo "Error: failed to create folder '$logFolder'"
    exit 4
fi

#create 4 files to time-stamp folder
cp $fileName ./$logFolder    #copy one to base backup folder
$fileName                    #exec integrate.test
echo $? > ./$logFolder/$fileName_clean.status
echo $($fileName) > ./$logFolder/$fileName_clean.stdout 2> ./$logFolder/$fileName_clean.stderr

#compare
if [ $dirName_exist -eq 1 ]; then
		echo "comparision -----------------------"
		diff --brief ./$logFolder $dirName
		if [ $? -eq 0 ];then
				echo "no differences!"
				exit 0
		else
				exit 5
		fi
		echo "------------------------------------"
	

elif [ $dirName_exist -eq 0 ]; then
		cp ./$logFolder/* $dirName
		exit 0
		
else
		echo "comparision error!"
		exit 6
fi
# echo -e "\n"
# find . -type f -name "test*" ! -name "*.*" -print -exec runtest.sh {} \;
echo -e "\n"
