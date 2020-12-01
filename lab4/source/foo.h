#ifndef FOO
#define FOO

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h> //подключение фиксированных типов данных

//int UsualMatrixMul(long long n, double &time_d);
int ProcessParameters(int argc, char *argv[],
                       long long &matrixSize, char* mulType,
                       long long &launchCnt, bool &bCheck,
                       long long &blockSize);
int TestsHandler(char* mulType, long long launchCnt,
                 long long matrixSize, bool bCheck,
                 long long blockSize);
long long GetCacheAlignment();

#endif
