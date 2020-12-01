#!/bin/bash

#очистить output.csv и записать туда заданную строку
echo "multiplication type;launch count;matrix size;block size;timer;average time;absError;relError;" > ../data/output.csv

cd ../source
make

for (( i=4; i<=256; i*=2 ))
do
./../source/main.exe -s 512 -t block -l 3 --block-size $i
done

