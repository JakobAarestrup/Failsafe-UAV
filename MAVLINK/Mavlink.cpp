#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <cstddef>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>

int main()
{

    int fd;
    int data;
    if ((fd = serialOpen("/dev/ttyS0", 9600)) < 0)
    {
        printf("Failed\n");
    }
    while (1)
    {
        data = serialGetchar(fd);
        printf("%c", data);
    }
    return 0;
}
