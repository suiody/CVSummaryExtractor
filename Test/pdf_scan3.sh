#!/bin/sh

#  mitie_scan.sh
#  
#
#
#!/bin/bash

# save and change IFS
OLDIFS=$IFS
IFS=$'\n'

# read all file name into an array
fileArray=($(find $* -type f))

# restore it
IFS=$OLDIFS
mkdir pdftotext3

# get length of an array
tLen=${#fileArray[@]}
start_ns=$(date +%s%N)
date1=$(date +"%s")
# use for loop read all filenames
for (( i=0; i<${tLen}; i++ ));
do
file=${fileArray[$i]}
echo $file

filename="${file##*/}"
newfilename=${filename%.*}
./CVSummaryExtractor "$file"
date2=$(date +"%s")
diff=$(($date2-$date1))
echo "$(($diff / 60)) minutes and $(($diff % 60)) seconds elapsed."
done
date2=$(date +"%s")
diff=$(($date2-$date1))
echo "$(($diff / 60)) minutes and $(($diff % 60)) seconds elapsed."



