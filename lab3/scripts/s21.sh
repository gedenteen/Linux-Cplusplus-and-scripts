#!/bin/bash

#очистить output.csv и записать туда заданную строку
echo "MemoryType;BlockSize;ElementType;BufferSize;LaunchNum;Timer;WriteTime;AverageWriteTime;WriteBandwidth;\
AbsError(write);RelError(write);ReadTime;AverageReadTime;ReadBandwidth;AbsError(read);RelError(read);" > ../data/output.csv

# $ getconf -a | grep CACHE
#dCache - cache lvl1 for data
#iCache - cache lvl1 for instructions
cache_line=$(getconf LEVEL1_DCACHE_LINESIZE) 
cache_lvl1=$(getconf LEVEL1_DCACHE_SIZE) 
cache_lvl2=$(getconf LEVEL2_CACHE_SIZE)
cache_lvl3=$(getconf LEVEL3_CACHE_SIZE)
cache_more=$(($cache_lvl3 + $cache_lvl2))

cd ../source
make
./../source/main.exe --launch-count 3 --memory-type RAM --block-size $cache_line
./../source/main.exe --launch-count 3 --memory-type RAM --block-size $cache_lvl1
./../source/main.exe --launch-count 3 --memory-type RAM --block-size $cache_lvl2
./../source/main.exe --launch-count 3 --memory-type RAM --block-size $cache_lvl3
./../source/main.exe --launch-count 3 --memory-type RAM --block-size $cache_more
