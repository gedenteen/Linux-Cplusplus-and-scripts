#!/bin/bash

#очистить output.csv и записать туда заданную строку
echo "multiplication type;launch count;matrix size;block size;threads count;timer;average time;absError;relError;" > ../data/output.csv

cd ../source
make

./../source/main.exe --check -s 4 -b 2 --multiplication-type usual --launch-count 1
./../source/main.exe --check -s 4 -b 2 --multiplication-type row_by_row --launch-count 1
./../source/main.exe --check -s 4 -b 2 --multiplication-type block --launch-count 1
./../source/main.exe --check -s 4 -b 2 --multiplication-type POSIX_Threads --launch-count 1 --threads-count 2 
./../source/main.exe --check -s 4 -b 2 --multiplication-type OpenMP --launch-count 1 --threads-count 2
