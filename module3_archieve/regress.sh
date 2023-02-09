#!/bin/bash
if [ $# -lt 2 ]; then                                  
   echo regress.sh \<dirname\> \<testfilename\>
   exit                                                                   
fi

if [ -f $1 ] || [ -h $1 ]; then
		echo regress.sh \<dirname\> \<testfilename\>
		exit 1
fi

for j in $2; do
		if [ ! -r $j ]; then
				echo regress.sh \<dirname\> \<testfilename\>
				echo cannot open file $j
				exit 1
		fi
done

DATE=$(date '+%Y%m%d.%H%M%S')
mkdir $DATE

if [ $? != 0 ]; then
		exit 1
fi
		
cp $j ./$DATE/$j
$j
echo $? > ./$DATE/$j.status
#echo $j 0< /dev/null
echo $j > ./$DATE/$j.stdout 2> ./$DATE/$j.stderr

if [ ! -d $1 ]; then
		mv $DATE $1
		exit 0
fi

		 
diff --brief $1 $DATE
if [ $? -eq 0 ]; then
		echo no difference
		exit 0
else
		echo have difference
		exit 1
fi

		


		
 




