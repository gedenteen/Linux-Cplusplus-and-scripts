#!/bin/bash

#очистить output.csv и записать туда заданную строку
echo "MemoryType;BlockSize;ElementType;BufferSize;LaunchNum;Timer;WriteTime;AverageWriteTime;WriteBandwidth;\
AbsError(write);RelError(write);ReadTime;AverageReadTime;ReadBandwidth;AbsError(read);RelError(read);" > ../data/output.csv

cd ../source
make
./../source/main.exe --launch-count 5 --memory-type SSD --block-size 12Kb --maximum-buffer
./../source/main.exe --launch-count 10 --memory-type SSD --block-size 12Kb --maximum-buffer
./../source/main.exe --launch-count 20 --memory-type SSD --block-size 12Kb --maximum-buffer
