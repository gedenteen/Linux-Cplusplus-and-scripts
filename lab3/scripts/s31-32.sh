#!/bin/bash

#очистить output.csv и записать туда заданную строку
echo "MemoryType;BlockSize;ElementType;BufferSize;LaunchNum;Timer;WriteTime;AverageWriteTime;WriteBandwidth;\
AbsError(write);RelError(write);ReadTime;AverageReadTime;ReadBandwidth;AbsError(read);RelError(read);" > ../data/output.csv

cd ../source
make
./../source/main.exe --launch-count 10 --memory-type RAM --block-size 8Kb
./../source/main.exe --launch-count 10 --memory-type RAM --block-size 64Kb
./../source/main.exe --launch-count 10 --memory-type RAM --block-size 256Kb
./../source/main.exe --launch-count 10 --memory-type SSD --block-size 8Kb
./../source/main.exe --launch-count 10 --memory-type SSD --block-size 64Kb
./../source/main.exe --launch-count 10 --memory-type SSD --block-size 256Kb
./../source/main.exe --launch-count 10 --memory-type flash --block-size 8Kb
./../source/main.exe --launch-count 10 --memory-type flash --block-size 64Kb
./../source/main.exe --launch-count 10 --memory-type flash --block-size 256Kb
