#include "foo.h"

int main(int argc, char *argv[]) {
    srand(time(0));
    //printf("sizeof(double)=%d \n", sizeof(double));
    ///инициализация параметров:
    long long matrixSize = 100;
    char* mulType = (char*) malloc(15);
    strcpy(mulType, "usual\0");
    long long launchCnt = 3;
    bool bCheck = false;
    long long blockSize = GetCacheAlignment();
    ProcessParameters(argc, argv, matrixSize, mulType, launchCnt, bCheck, blockSize);
    ///проверка параметров:
    printf("--- arguments of main(): --- \n");
    printf("matrixSize = %lld \n", matrixSize);
    printf("mulType = %s \n", mulType);
    printf("launchCnt = %lld \n", launchCnt);
    printf("bCheck = %d \n", bCheck ? 1 : 0);
    if (strcmp("block", mulType) == 0)
        printf("blockSize = %lld \n", blockSize);
    ///запуск:
    TestsHandler(mulType, launchCnt, matrixSize, bCheck, blockSize);

    free(mulType);
    return 0;
}
