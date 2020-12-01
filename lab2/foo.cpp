#include "foo.h"

void test1_1(int mx, double &time) { ///типовая задача (тест)
    int arr[mx][mx];
    int i, j;
    for (i = 0; i < mx; i++) {
        for (j = 0; j < mx; j++) {
            arr[i][j] = rand() % mx;
        }
    }
    int koef = rand() % mx;

    clock_t start, stop;
    start = clock();
    for (i = 0; i < mx; i++) {
        for (j = 0; j < mx; j++) {
            arr[i][j] *= koef;
        }
    }
    stop = clock();
    time = ((double)(stop - start)) / CLOCKS_PER_SEC;
}

void test1_2(int mx, double &time) { ///типовая задача с другим типом данных
    long long arr[mx][mx];
    int i, j;
    for (i = 0; i < mx; i++) {
        for (j = 0; j < mx; j++) {
            arr[i][j] = rand() % mx;
        }
    }
    long long koef = rand() % mx;

    clock_t start, stop;
    start = clock();
    for (i = 0; i < mx; i++) {
        for (j = 0; j < mx; j++) {
            arr[i][j] *= koef;
        }
    }
    stop = clock();
    time = ((double)(stop - start)) / CLOCKS_PER_SEC;
}

void test1_3(int mx, double &time) { ///типовая задача с другим типом данных
    double arr[mx][mx];
    int i, j;
    for (i = 0; i < mx; i++) {
        for (j = 0; j < mx; j++) {
            arr[i][j] = rand() % mx * (rand() % 10 * 0.1);
        }
    }
    double koef = rand() % mx * (rand() % 10 * 0.1);

    clock_t start, stop;
    start = clock();
    for (i = 0; i < mx; i++) {
        for (j = 0; j < mx; j++) {
            arr[i][j] *= koef;
        }
    }
    stop = clock();
    time = ((double)(stop - start)) / CLOCKS_PER_SEC;
}

void benchmark(int num, long long n, int matrix_mx,
               double &avg_time, double &dispersion,
               double &abs_error, double &rel_error)
{
    double summand1 = 0, summand2 = 0;
    double x; ///время выполнения только типовой задачи

    for (long long i = 0; i < n; i++) {
        switch (num) { ///выбор типовой задачи (теста)
            case 0:
                test1_1(matrix_mx, x);
                break;
            case 1:
                test1_2(matrix_mx, x);
                break;
            case 2:
                test1_3(matrix_mx, x);
                break;
            default:
                printf("ERROR: wrong \"num\" in benchmark() \n");
                break;
        } ///конец switch
        summand1 += x * x; ///первое слагаемое для дисперсии
        summand2 += x; ///2-ое слагаемое
    }
    summand1 /= n;
    summand2 /= n;
    avg_time = summand2; ///sum(x[i]) / n == среднее время
    summand2 *= summand2;
    dispersion = summand1 - summand2; ///дисперсия (точность измерения времени)
    abs_error = sqrt(dispersion); ///среднее квадратическое отклонение (погрешность)
    rel_error = dispersion / avg_time; ///относительная погрешность
}

void get_cpu_name(char cpuname[]) {
    FILE *fcpu;
    if ((fcpu = fopen("/proc/cpuinfo", "r")) == NULL) {
        printf("Can't open /proc/cpuinfo \n");
        return;
    }
    size_t m = 0;
    char *line = NULL;
    while (getline(&line, &m, fcpu) > 0) {
        if (strstr(line, "model name")) {
            strcpy(cpuname, &line[13]);
            break;
        }
    }
    for (int i = 0; i < 50; i++) {
        if (cpuname[i] == '\n')
            cpuname[i] = '\0';
    }
    fclose(fcpu);
}
