#!/bin/bash

#очистить output.csv и записать туда заданную строку
echo "multiplication type;launch count;matrix size;block size;timer;average time;absError;relError;" > ../data/output.csv

cd ../source
make

for (( i=400; i<=2000; i+=400 ))
do
./../source/main.exe --matrix-size $i --multiplication-type usual --launch-count 3
done

