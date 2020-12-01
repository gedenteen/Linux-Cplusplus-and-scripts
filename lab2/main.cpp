#include "foo.h"

int main() {
    srand(time(0));
    int cnt_tests = 3, i; ///количество тестов
    long long n[cnt_tests];
    n[0] = n[1] = n[2] = 1000;
    double avg_time[cnt_tests], dispersion[cnt_tests];
    double abs_error[cnt_tests], rel_error[cnt_tests];
    int matrix_mx = 100;

    for (i = 0; i < cnt_tests; i++) {
        benchmark(i, n[i], matrix_mx, avg_time[i], dispersion[i], abs_error[i], rel_error[i]);
    }

    ///получение названия процессора
    char cpuname[50] = {'\0'};
    get_cpu_name(cpuname);
    //printf("%s %d \n", cpuname, (int)sizeof(cpuname));

    ///запись в output.csv
    FILE *fout;
    if ((fout = fopen("output.csv", "w")) == NULL) {
        printf("Can't open output.csv \n");
        return 1;
    }
    //fprintf(fout, "PModel;Task;OpType;Opt;LNum;InsCount;Timer;AvTime;AbsErr;RelErr;TaskPerf\n");
    for (i = 0; i < cnt_tests; i++) {
        fprintf(fout, cpuname); ///proc
        fprintf(fout, ";");
        switch (i) {
            case 0:
                fprintf(fout, "num * matrix;int;None;");
                break;
            case 1:
                fprintf(fout, "num * matrix;long long;None;");
                break;
            case 2:
                fprintf(fout, "num * matrix;double;None;");
                break;
            default:
                printf("ERROR: wrong \"num\" while writing a file \n");
                break;
        }
        fprintf(fout, "%lld;", n[i]); ///lanch number
        switch (i) { ///instruction count
            case 0:
                fprintf(fout, "%d;", matrix_mx * 3 + 2);
                break;
            case 1:
                fprintf(fout, "%d;", matrix_mx * 3 + 2);
                break;
            case 2:
                fprintf(fout, "%d;", matrix_mx * 3 + 2);
                break;
        }
        fprintf(fout, "clock();"); ///Timer
        fprintf(fout, "%g;", avg_time[i]);
        fprintf(fout, "%g;", abs_error[i]);
        fprintf(fout, "%g%%;", rel_error[i] * 100); ///относительная погрешность в %
        fprintf(fout, "%g;", 1 / avg_time[i]);
        fprintf(fout, "\n");
    }


    fclose(fout);
    return 0;
}
