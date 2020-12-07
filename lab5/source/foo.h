#ifndef FOO
#define FOO

#include <math.h> //для вычисления погрешностей
#include <stdio.h> //ввод/вывод
#include <stdlib.h> //рандом
#include <time.h> //измерение времены
#include <string.h> //Для сравнения строк
#include <pthread.h> //POSIX threads
#include <omp.h> //OpemMP

typedef struct a {
    double *matrix1;
    double *matrix2;
    double *matrixRes;
    int matrixSize;
    int from;
    int to;
} argsForThread;

int ProcessParameters(int argc, char *argv[],
                      int &matrixSize, char* mulType,
                      int &launchCnt, bool &bCheck,
                      int &blockSize, int &threadsCnt);
int TestsHandler(char* mulType, int launchCnt,
                 int matrixSize, bool bCheck,
                 int blockSize, int &threadsCnt);
long long GetCacheAlignment();

#endif
