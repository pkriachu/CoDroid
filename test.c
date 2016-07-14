#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char argv[]) {
    struct tm newtime;
    time_t ltime;
    char buf[50];

    ltime = time(&ltime);
    localtime_r(&ltime, &newtime);
    printf("%s", asctime_r(&newtime, buf));
}
