#ifndef FOO
#define FOO

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void benchmark(int num, long long n, int matrix_mx,
               double &avg_time, double &dispersion,
               double &abs_error, double &rel_error);
void get_cpu_name(char cpuname[]);

#endif
