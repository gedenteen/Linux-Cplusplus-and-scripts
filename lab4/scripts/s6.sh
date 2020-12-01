#!/bin/bash

#очистить output.csv и записать туда заданную строку
echo "multiplication type;launch count;matrix size;block size;timer;average time;absError;relError;" > ../data/output.csv

cd ../source
make

for (( i=256; i<=1024; i+=256 ))
do
./../source/main.exe --matrix-size $i --multiplication-type usual --launch-count 3
./../source/main.exe --matrix-size $i --multiplication-type row_by_row --launch-count 3
./../source/main.exe --matrix-size $i --multiplication-type block --launch-count 3
done

