#include "foo.h"

int Process_parameters(int argc, char *argv[], char* mem_type,
                       long long &mem_size, long long &launch_cnt,
                       bool &max_buffer)
{
    int i;
    for (i = 1; i < argc; i++)
    {
        ///если тип памяти:
        if (strcmp("-m", argv[i]) == 0 ||
            strcmp("--memory-type", argv[i]) == 0)
        {
            i++;
            if (strcmp("RAM", argv[i]) == 0 ||
                strcmp("SSD", argv[i]) == 0 ||
                strcmp("flash", argv[i]) == 0 ||
                strcmp("SWAP", argv[i]) == 0)
            {
                strcpy(mem_type, argv[i]);
            }
            else {
                printf("Error in arguments of main(): incorrect value for --memory-type \n");
                return 1;
            }
        }
        ///если размер блока данных:
        else if (strcmp("-b", argv[i]) == 0 ||
                 strcmp("--block-size", argv[i]) == 0)
        {
            i++;
            size_t len = strlen(argv[i]);
            int coefficient = 1;
            if (argv[i][len-1] == 'b') { ///проверка на Кб или Мб
                if (argv[i][len-2] == 'K')
                    coefficient = 1024;
                else if (argv[i][len-2] == 'M')
                    coefficient = 1024 * 1024;
                else {
                    printf("Error in arguments of main(): incorrect description of unit of measure byte \n");
                    return 1;
                }
                strncpy(argv[i], argv[i], len - 2); ///убрать единицы измерения
            }
            mem_size = atoll(argv[i]) * coefficient; ///приведение строки в long long int
        }
        ///если число испытаний:
        else if (strcmp("-l", argv[i]) == 0 ||
                 strcmp("--launch-count", argv[i]) == 0)
        {
            i++;
            launch_cnt = atoll(argv[i]); ///приведение строки в long long int
            if (launch_cnt == 0) {
                printf("Error in arguments of main(): incorrect value for --launch-count \n");
                return 1;
            }
        }
        ///если задана опция на максимально возможный буфер:
        else if (strcmp("--maximum-buffer", argv[i]) == 0)
        {
            max_buffer = true;
        }
        else {
            printf("Error in arguments of main(): incorrect value (ind = %d) \n", i);
            return 1;
        }
    }

    return 0;
}

int Test_RAM(long long mem_size, bool max_buffer,
             double &time_write, double &time_read)
 {
    uint8_t* arr1 = (uint8_t*) malloc(mem_size); ///тип данных - фиксированный 1 байт [0..255]
    long long i;
    for (i = 0; i < mem_size; i++) {
        arr1[i] = rand() % 256;
    }
    ///подсчет времени на запись массива:
    uint8_t* arr2 = (uint8_t*) malloc(mem_size);
    clock_t start, stop;
    if (max_buffer) {
        start = clock();
        memcpy(arr2, arr1, mem_size); ///копировать содержимое одной области памяти в другую
        stop = clock();
    }
    else {
        start = clock();
        for (i = 0; i < mem_size; i++) {
            arr2[i] = arr1[i];
        }
        stop = clock();
    }
    time_write = ((double)(stop - start)) / CLOCKS_PER_SEC;
    time_read = time_write; ///чтение и запись - идентичные операции
    ///проверка:
//    for(i = 0; i < mem_size; i++) {
//        printf("i=%lld arr1[i]=%hhu arr2[i]=%hhu \n", i, arr1[i], arr2[i]);
//    }
    free(arr1);
    free(arr2);
    return 0;
}

int Test_storage_device(char* mem_type, long long mem_size, bool max_buffer,
                        double &time_write, double &time_read)
{
    uint8_t* arr = (uint8_t*) malloc(mem_size); ///тип данных - фиксированный 1 байт [0..255]
    long long i;
    for (i = 0; i < mem_size; i++) {
        arr[i] = rand() % 256;
    }
    ///открытие файла на накопителе для записи:
    FILE *fp;
    if (strcmp("SSD", mem_type) == 0) {///SSD
        if ((fp = fopen("test_SSD.txt", "w")) == NULL) {
            printf("Error in Test_storage_device(): can't open file test_SSD.txt \n");
            return 1;
        }
    }
    else if (strcmp("flash", mem_type) == 0) { ///USB
        if ((fp = fopen("/media/dmitry/SMARTBUYUSB/test_flash.txt", "w")) == NULL) {
            printf("Error in Test_storage_device(): can't open file test_flash.txt in USB \n");
            return 1;
        }
    }
    else { ///swap
        if ((fp = fopen("/swapfile", "wb")) == NULL) {
            printf("Error in Test_storage_device(): can't open file swapfile \n");
            return 1;
        }
    }
    ///запись массива:
    clock_t start, stop;
    if (max_buffer) {
        start = clock();
        fwrite(arr, mem_size, 1, fp);
        stop = clock();
    }
    else {
        start = clock();
        for (i = 0; i < mem_size; i++) {
            fprintf(fp, "%hhu ", arr[i]);
        }
        stop = clock();
    }
    time_write = ((double)(stop - start)) / CLOCKS_PER_SEC;
    ///открытие файла на накопителе для чтения:
    fclose(fp);
    if (strcmp("SSD", mem_type) == 0) {///SSD
        if ((fp = fopen("test_SSD.txt", "r")) == NULL) {
            printf("Error in Test_storage_device(): can't open file test_SSD.txt \n");
            return 1;
        }
    }
    else if (strcmp("flash", mem_type) == 0) { ///USB
        if ((fp = fopen("/media/dmitry/SMARTBUYUSB/test_flash.txt", "r")) == NULL) {
            printf("Error in Test_storage_device(): can't open file test_flash.txt in USB \n");
            return 1;
        }
    }
    else { ///swap
        if ((fp = fopen("/swapfile", "r")) == NULL) {
            printf("Error in Test_storage_device(): can't open file swapfile \n");
            return 1;
        }
    }
    ///чтение массива:
    if (max_buffer) {
        start = clock();
        fread(arr, mem_size, 1, fp);
        stop = clock();
    }
    else {
        start = clock();
        for (i = 0; i < mem_size; i++) {
            fscanf(fp, "%hhu", &arr[i]);
        }
        stop = clock();
    }
    time_read = ((double)(stop - start)) / CLOCKS_PER_SEC;

    free(arr);
    fclose(fp);
    return 0;
}


int Write_to_csv(char* mem_type, long long launch_cnt,
                 long long mem_size, bool max_buffer,
                 double time_write[], double avg_time_write,
                 double abs_error_write, double rel_error_write,
                 double time_read[], double avg_time_read,
                 double abs_error_read, double rel_error_read)
{
    ///открытие output.csv:
    FILE *fout;
    if ((fout = fopen("../data/output.csv", "a")) == NULL) {
        printf("Error in Write_to_csv(): can't open output.csv \n");
        return 1;
    }
    //fprintf(fout, "MemoryType;BlockSize;ElementType;BufferSize;LaunchNum;Timer;WriteTime;AverageWriteTime;WriteBandwidth;AbsError(write);RelError(write);ReadTime;AverageReadTime;ReadBandwidth;AbsError(read);RelError(read);\n");
    for (long long i = 0; i < launch_cnt; i++) {
        fprintf(fout, "%s;", mem_type); ///MemoryType
        fprintf(fout, "%lld;", mem_size); ///BlockSize
        fprintf(fout, "uint8_t;"); ///ElementType
        if (max_buffer) ///BufferSize
            fprintf(fout, "%lld;", mem_size);
        else
            fprintf(fout, "1;");
        fprintf(fout, "%lld;", i+1); ///LaunchNum
        fprintf(fout, "clock();"); ///Timer
        fprintf(fout, "%e;", time_write[i]); ///WriteTime
        fprintf(fout, "%e;", avg_time_write); ///AverageWriteTime
        fprintf(fout, "%e;", (double)mem_size / avg_time_write * 1e6); ///WriteBandwidth
        fprintf(fout, "%e;", abs_error_write); ///AbsError(write)
        fprintf(fout, "%e%%;", rel_error_write); ///RelError(write)
        fprintf(fout, "%e;", time_read[i]); ///ReadTime
        fprintf(fout, "%e;", avg_time_read); ///AverageReadTime
        fprintf(fout, "%e;", (double)mem_size / avg_time_read * 1e6); ///ReadBandwidth
        fprintf(fout, "%e;", abs_error_read); ///AbsError(read)
        fprintf(fout, "%e%%;", rel_error_read); ///RelError(read)
        fprintf(fout, "\n");
    }

    return 0;
}

int Tests_handler(char* mem_type, long long launch_cnt,
               long long mem_size, bool max_buffer)
{
    ///начальные переменные для измерений:
    double summand1_write = 0, summand2_write = 0;
    double summand1_read = 0, summand2_read = 0;
    double time_write[launch_cnt]; ///время выполнения записи массива
    double time_read[launch_cnt]; ///время выполнения чтения массива
    ///измерения (тестирование):
    for (long long i = 0; i < launch_cnt; i++) {
        if (strncmp("RAM", mem_type, 3) == 0) { ///если RAM
            if (Test_RAM(mem_size, max_buffer, time_write[i], time_read[i]) == 1)
                return 1;
        }
        else { ///если SSD или flash или SWAP
            if (Test_storage_device(mem_type, mem_size, max_buffer,
                time_write[i], time_read[i]) == 1)
                return 1;
        }
        summand1_write += time_write[i] * time_write[i]; ///первое слагаемое для дисперсии
        summand2_write += time_write[i]; ///2-ое слагаемое
        summand1_read += time_read[i] * time_read[i]; ///первое слагаемое для дисперсии
        summand2_read += time_read[i]; ///2-ое слагаемое
    }
    ///заключительные переменные для измерений записи:
    summand1_write /= launch_cnt;
    summand2_write /= launch_cnt;
    double avg_time_write = summand2_write; ///сумма времени всех тестов / n == среднее время
    summand2_write *= summand2_write;
    double dispersion_write = summand1_write - summand2_write; ///дисперсия (точность измерения времени)
    double abs_error_write = sqrt(dispersion_write); ///среднее квадратическое отклонение (погрешность)
    double rel_error_write = dispersion_write / avg_time_write * 100; ///относительная погрешность в %
    ///заключительные переменные для измерений чтения:
    summand1_read /= launch_cnt;
    summand2_read /= launch_cnt;
    double avg_time_read = summand2_read; ///сумма времени всех тестов / n == среднее время
    summand2_read *= summand2_read;
    double dispersion_read = summand1_read - summand2_read; ///дисперсия (точность измерения времени)
    double abs_error_read = sqrt(dispersion_read); ///среднее квадратическое отклонение (погрешность)
    double rel_error_read = dispersion_read / avg_time_read * 100; ///относительная погрешность в %

    Write_to_csv(mem_type, launch_cnt, mem_size, max_buffer,
                 time_write, avg_time_write,
                 abs_error_write, rel_error_write,
                 time_read, avg_time_read,
                 abs_error_read, rel_error_read);
    return 0;
}
