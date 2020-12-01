#include "foo.h"

int ProcessParameters(int argc, char *argv[],
                      int &matrixSize, char* mulType,
                      int &launchCnt, bool &bCheck,
                      int &blockSize)
{
    int i;
    for (i = 1; i < argc; i++)
    {
        ///если размер матрицы:
        if (strcmp("-s", argv[i]) == 0 ||
            strcmp("--matrix-size", argv[i]) == 0)
        {
            i++;
            matrixSize = atoi(argv[i]); ///приведение строки в long long int
            if (matrixSize == 0) {
                printf("Error in arguments of main(): incorrect value for --launch-count \n");
                return 1;
            }
        }
        ///если тип умножения:
        else if (strcmp("-t", argv[i]) == 0 ||
                 strcmp("--multiplication-type", argv[i]) == 0)
        {
            i++;
            if (strcmp("usual", argv[i]) == 0 ||
                strcmp("row_by_row", argv[i]) == 0 ||
                strcmp("block", argv[i]) == 0 ||
                strcmp("POSIX_Threads", argv[i]) == 0 ||
                strcmp("OpenMP", argv[i]) == 0)
            {
                strcpy(mulType, argv[i]);
            }
            else {
                printf("Error in arguments of main(): incorrect value for --multiplication-type \n");
                return 1;
            }
        }
        ///если число испытаний:
        else if (strcmp("-l", argv[i]) == 0 ||
                 strcmp("--launch-count", argv[i]) == 0)
        {
            i++;
            launchCnt = atoi(argv[i]); ///приведение строки в long long int
            if (launchCnt == 0) {
                printf("Error in arguments of main(): incorrect value for --launch-count \n");
                return 1;
            }
        }
        ///если включение проверки умножения:
        else if (strcmp("-c", argv[i]) == 0 ||
                 strcmp("--check", argv[i]) == 0)
        {
            bCheck = true;
        }
        else if (strcmp("-b", argv[i]) == 0 ||
                 strcmp("--block-size", argv[i]) == 0)
        {
            i++;
            blockSize = atoi(argv[i]); ///приведение строки в long long int
            if (blockSize == 0) {
                printf("Error in arguments of main(): incorrect value for --block-size \n");
                return 1;
            }
        }
    }

    return 0;
}

int min(int a, int b) {
    if (a < b)
        return a;
    else
        return b;
}

long long GetCacheAlignment() {
    FILE *fcpu;
    if ((fcpu = fopen("/proc/cpuinfo", "r")) == NULL) {
        printf("Error: can't open /proc/cpuinfo \n");
        return -1;
    }
    size_t m = 0;
    char *line = NULL, *temp = (char*) malloc(50);
    while (getline(&line, &m, fcpu) > 0) {
        if (strstr(line, "cache_alignment")) {
            strcpy(temp, &line[18]);
            break;
        }
    }


    for (int i = 0; i < 50; i++) {
        if (temp[i] == ' ' || temp[i] == '\n') {
            temp[i] = '\0';
            //strncpy(temp, temp, i);
            //break;
        }
    }
    //printf("temp=%s|\n", temp);
    int val = atoi(temp);
    if (val == 0) {
        printf("Error in GetCacheAlignment(): can't atoll \n");
        return -1;
    }

    fclose(fcpu);
    free(temp);
    return val;
}

int PrintMatrix(double **matrix, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            //printf("[%lld][%lld]=", i, j);
            printf("%.6f ", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}

int PrintMatrix(double *matrix, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            //printf("[%lld][%lld]=", i, j);
            printf("%.2f ", matrix[i * n + j]);
        }
        printf("\n");
    }
    return 0;
}

int MatrixMul(int mulType_i, int matrixSize,
              bool bCheck, double &time_d) {
    int i, j, k;
    ///выделение памяти под матрицы:
    double **matrix1 = new double*[matrixSize];
    double **matrix2 = new double*[matrixSize];
    double **matrixRes = new double*[matrixSize];
    for (i = 0; i < matrixSize; i++) {
        matrix1[i] = new double[matrixSize];
        matrix2[i] = new double[matrixSize];
        matrixRes[i] = new double[matrixSize];
    }
    ///заполнение матриц случайныи числами:
    for (i = 0; i < matrixSize; i++) {
        for (j = 0; j < matrixSize; j++) {
            matrix1[i][j] = rand() / 123456 + (double)rand() / RAND_MAX;
            matrix2[i][j] = rand() / 123456 + (double)rand() / RAND_MAX;
            matrixRes[i][j] = 0;
        }
    }
    ///проверка:
    if (bCheck) {
        printf("before multiplication: \n");
        PrintMatrix(matrix1, min(3, matrixSize));
        printf("\n");
        PrintMatrix(matrix2, min(3, matrixSize));
        printf("\n");
        PrintMatrix(matrixRes, min(3, matrixSize));
        printf("\n");
    }
    ///для вычисления времени:
    clock_t start, stop;
    long long time_i = 0;
    start = clock();
    ///умножение матриц по строке и столбцу:
    if (mulType_i == 1) { ///обычное умножение матриц
        for (i = 0; i < matrixSize; i++)
        for (j = 0; j < matrixSize; j++)
        for (k = 0; k < matrixSize; k++) {
            ///если замерять время здесь, то программа многократно замедляется
            matrixRes[i][j]  += matrix1[i][k] * matrix2[k][j];
        }
    }
    else if (mulType_i == 2) { ///построчное умножение матриц
        for (i = 0; i < matrixSize; i++)
        for (k = 0; k < matrixSize; k++)
        for (j = 0; j < matrixSize; j++) {
            matrixRes[i][j]  += (double)matrix1[i][k] * matrix2[k][j];
        }
    }
    else {
        printf("Error in MatrixMul(), wrong mulType_i");
    }
    ///вычисление времени:
    stop = clock();
    time_i += stop - start; ///время в тактах
    time_d = (double)time_i / CLOCKS_PER_SEC; ///время в секундах
    ///проверка:
    if (bCheck) {
        printf("\nafter multiplication: \n");
        printf("time_d=%f \n", time_d);
        PrintMatrix(matrix1, min(3, matrixSize));
        printf("\n");
        PrintMatrix(matrix2, min(3, matrixSize));
        printf("\n");
        PrintMatrix(matrixRes, min(3, matrixSize));
        printf("\n");
    }
    ///освобождение памяти:
    for (i = 0; i < matrixSize; i++) {
        delete(matrix1[i]);
        delete(matrix2[i]);
        delete(matrixRes[i]);
    }
    delete(matrix1);
    delete(matrix2);
    delete(matrixRes);
    return 0;
}

typedef struct a {
    double *matrix1;
    double *matrix2;
    double *matrixRes;
    int matrixSize;
    int from;
    int to;
} argsForThread;

void* MatrixMulForThread(void* voidpArgs)
{
    argsForThread* pArgs = (argsForThread*) voidpArgs;

    for (int i = pArgs->from; i < pArgs->to; i++) {
        printf("i=%d ", i);
        for (int j = 0; j < pArgs->matrixSize; j++)
        for (int k = 0; k < pArgs->matrixSize; k++) {
            pArgs->matrixRes[i * pArgs->matrixSize + j]  +=
                pArgs->matrix1[i * pArgs->matrixSize + k] *
                pArgs->matrix2[k * pArgs->matrixSize + j];
        }
    }

    return 0;
}

int MatrixBlockMul(int mulType_i, int blockSize,
                   int matrixSize,
                   bool bCheck, double &time_d) {
    int i, j, k;
    ///выделение памяти под матрицы:
    double *matrix1 = new double[matrixSize * matrixSize];
    double *matrix2 = new double[matrixSize * matrixSize];
    double *matrixRes = new double[matrixSize * matrixSize];
    ///заполнение матриц:
    if (bCheck) for (i = 0; i < matrixSize*matrixSize; i++) {
        matrix1[i] = i;
        matrix2[i] = i;
        matrixRes[i] = 0;
    }
    else for (i = 0; i < matrixSize*matrixSize; i++) {
        matrix1[i] = rand() / 123456 + (double)rand() / RAND_MAX;
        matrix2[i] = rand() / 123456 + (double)rand() / RAND_MAX;
        matrixRes[i] = 0;
    }
    ///проверка:
    if (bCheck) {
        printf("matrix1: \n");
        PrintMatrix(matrix1, matrixSize);
        printf("matrix2: \n");
        PrintMatrix(matrix2, matrixSize);
    }
    ///для вычисления времени:
    clock_t start, stop;
    long long time_i = 0;
    start = clock();
    ///умножение матриц по строке и столбцу:
    double *m1, *m2, *mRes; ///копии матриц
    int i0, j0, k0;
    if (mulType_i == 3) { ///блочное умножение матриц
        for (i = 0; i < matrixSize; i += blockSize)
        for (j = 0; j < matrixSize; j += blockSize)
        for (k = 0; k < matrixSize; k += blockSize) {
            for (i0 = 0, mRes = (matrixRes + i * matrixSize + j),
                 m1 = (matrix1 + i * matrixSize + k); i0 < blockSize;
                 ++i0, mRes += matrixSize, m1 += matrixSize)
            {
                for (k0 = 0, m2 = (matrix2 + k * matrixSize + j);
                     k0 < blockSize; ++k0, m2 += matrixSize)
                {
                    for (j0 = 0; j0 < blockSize; ++j0)
                        mRes[j0] += m1[k0] * m2[j0];
                }
            }
        }
    }
    if (mulType_i == 4) { ///POSIX Threads
        printf("aaaa 4 ");
        ///создать указатель на структуру, которая содержит данные для потока:
        argsForThread* pArgs = (argsForThread*) malloc(sizeof(argsForThread));
        pArgs->matrix1 = matrix1;
        pArgs->matrix2 = matrix2;
        pArgs->matrixRes = matrixRes;
        pArgs->matrixSize = matrixSize;
        pArgs->from = 0;
        pArgs->to = matrixSize;
        ///создание потоков:
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, MatrixMulForThread, pArgs);
        ///подождать поток
        int status = pthread_join(thread_id, NULL);
        if (status == 0)
            printf("free thread \n");
        else
            printf("error, MatrixMulForThread() failed \n");

    }
    if (mulType_i == 5) { ///OpenMP
        //#pragma omp parallel
        {
            start = clock();
            #pragma omp parallel for shared(matrix1, matrix2, matrixRes) private(j, k) num_threads(2)
            #pragma omp for schedule(static)
            for (i = 0; i < matrixSize; i++) {
                for (j = 0; j < matrixSize; j++)
                for (k = 0; k < matrixSize; k++) {
                    matrixRes[i * matrixSize + j]  +=
                        matrix1[i * matrixSize + k] * matrix2[k * matrixSize + j];
                }
            }
            stop = clock();
            time_d = (double)(stop - start) / CLOCKS_PER_SEC;
            printf("stop - start = %f \n", time_d);
        }
    }
    ///проверка:
    if (bCheck) {
        printf("matrixRes: \n");
        PrintMatrix(matrixRes, matrixSize);
    }
    ///вычисление времени:
    stop = clock();
    time_i += stop - start; ///время в тактах
    if (mulType_i != 5)
        time_d = (double)time_i / CLOCKS_PER_SEC; ///время в секундах
    ///освобождение памяти:
    delete(matrix1);
    delete(matrix2);
    delete(matrixRes);
    return 0;
}


int WriteToCSV(char* mulType, int launchCnt, int matrixSize,
               int blockSize,
               double avgTime, double absError, double relError)
{
    ///открытие output.csv:
    FILE *fout;
    if ((fout = fopen("../data/output.csv", "a")) == NULL) {
        printf("Error in Write_to_csv(): can't open output.csv \n");
        return 1;
    }

    fprintf(fout, "%s;", mulType);
    fprintf(fout, "%d;", launchCnt);
    fprintf(fout, "%d;", matrixSize);
    fprintf(fout, "%d;", blockSize);
    fprintf(fout, "clock();"); ///Timer
    fprintf(fout, "%e;", avgTime);
    fprintf(fout, "%e;", absError);
    fprintf(fout, "%e%%;", relError);
    fprintf(fout, "\n");

    return 0;
}

int TestsHandler(char* mulType, int launchCnt,
                 int matrixSize, bool bCheck,
                 int blockSize)
{
    ///начальные переменные для измерений:
    double summand1 = 0, summand2 = 0;
    double time_d[launchCnt];
    ///измерения (тестирование):
    for (int i = 0; i < launchCnt; i++) {
        if (strcmp("usual", mulType) == 0) {
            MatrixMul(1, matrixSize, bCheck, time_d[i]);
        }
        if (strcmp("row_by_row", mulType) == 0) {
            MatrixMul(2, matrixSize, bCheck, time_d[i]);
        }
        if (strcmp("block", mulType) == 0) {
            MatrixBlockMul(3, blockSize, matrixSize, bCheck, time_d[i]);
        }
        if (strcmp("POSIX_Threads", mulType) == 0) {
            MatrixBlockMul(4, blockSize, matrixSize, bCheck, time_d[i]);
        }
        if (strcmp("OpenMP", mulType) == 0) {
            MatrixBlockMul(5, blockSize, matrixSize, bCheck, time_d[i]);
        }
        summand1 += time_d[i] * time_d[i];
        summand2 += time_d[i];
    }
    ///заключительные переменные для измерений записи:
    summand1 /= launchCnt;
    summand2 /= launchCnt;
    double avgTime = summand2; ///сумма времени всех тестов / n == среднее время
    summand2 *= summand2;
    double dispersion = summand1 - summand2; ///дисперсия (точность измерения времени)
    double absError = sqrt(dispersion); ///среднее квадратическое отклонение (погрешность)
    double relError = dispersion / avgTime * 100; ///относительная погрешность в %

    WriteToCSV(mulType, launchCnt, matrixSize, blockSize, avgTime, absError, relError);
    return 0;
}
