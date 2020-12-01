#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int max = 3000; // сколько МБ занимать
    int wait = 60 ; // сколько времени ждать
    int mb = 0 ;
    char* buffer;

    while((buffer=(char*)malloc(1024*1024)) != NULL && mb != max) {
        memset(buffer, 0, 1024*1024);
        mb++;
    }
    printf("Allocated %d MB\n", mb);

    while ( wait > 0 ) {
        printf("\rHold memory for %d seconds", wait);
        fflush( stdout );
        sleep(1);
        wait-- ;
    }
    printf("\nMemory is freed\n");
    return 0;
}
