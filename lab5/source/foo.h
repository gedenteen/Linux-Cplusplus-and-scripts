#ifndef FOO
#define FOO

#include <math.h> //для вычисления погрешностей
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> //Для сравнения строк
//#include <stdint.h> //подключение фиксированных типов данных
#include <pthread.h>
#include <omp.h>

//int UsualMatrixMul(long long n, double &time_d);
int ProcessParameters(int argc, char *argv[],
                       int &matrixSize, char* mulType,
                       int &launchCnt, bool &bCheck,
                       int &blockSize);
int TestsHandler(char* mulType, int launchCnt,
                 int matrixSize, bool bCheck,
                 int blockSize);
long long GetCacheAlignment();

#endif
