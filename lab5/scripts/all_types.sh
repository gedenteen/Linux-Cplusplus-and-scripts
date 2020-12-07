#!/bin/bash

#очистить output.csv и записать туда заданную строку
echo "multiplication type;launch count;matrix size;block size;threads count;timer;average time;absError;relError;" > ../data/output.csv

cd ../source
make

./../source/main.exe --matrix-size 1024 --multiplication-type usual --launch-count 3
./../source/main.exe --matrix-size 1024 --multiplication-type row_by_row --launch-count 3
./../source/main.exe --matrix-size 1024 --multiplication-type block --launch-count 3
./../source/main.exe --matrix-size 1024 --multiplication-type POSIX_Threads --launch-count 3 --threads-count 2
./../source/main.exe --matrix-size 1024 --multiplication-type OpenMP --launch-count 3 --threads-count 2
