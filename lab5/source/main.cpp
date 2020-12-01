#include "foo.h"

int main(int argc, char *argv[]) {
    srand(time(0));
    //printf("sizeof(double)=%d \n", sizeof(double));
    ///инициализация параметров:
    int matrixSize = 100;
    char* mulType = (char*) malloc(15);
    strcpy(mulType, "usual\0");
    int launchCnt = 3;
    bool bCheck = false;
    int blockSize = GetCacheAlignment();
    int threadsCnt = 1;
    ProcessParameters(argc, argv, matrixSize, mulType,
                      launchCnt, bCheck, blockSize, threadsCnt);
    ///проверка параметров:
    printf("--- arguments of main(): --- \n");
    printf("matrixSize = %d \n", matrixSize);
    printf("mulType = %s \n", mulType);
    printf("launchCnt = %d \n", launchCnt);
    printf("bCheck = %d \n", bCheck ? 1 : 0);
    if (strcmp("block", mulType) == 0)
        printf("blockSize = %d \n", blockSize);
    if (threadsCnt > 1)
        printf("threadsCnt = %d \n", threadsCnt);
    ///запуск:
    TestsHandler(mulType, launchCnt, matrixSize, bCheck, blockSize, threadsCnt);

    free(mulType);
    return 0;
}
