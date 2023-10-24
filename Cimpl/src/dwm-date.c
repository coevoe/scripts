#include <stdio.h>
#include <time.h>

#define BUFF 100

int main(int argc, char** argv) {
    char buff[BUFF];
    struct tm* ptr;
    time_t t;

    t = time(NULL);
    ptr = localtime(&t);
    strftime(buff, BUFF, "%H:%M:%S %d.%m.%y", ptr);

    printf("%s\n", buff);
}
