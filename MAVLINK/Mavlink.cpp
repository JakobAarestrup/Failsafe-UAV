#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <cstddef>
#include <unistd.h>

int main()
{

    int fd;
    if ((fd = serialOpen("/dev/ttyS0", 9600)) < 0)
    {
        printf("Failed\n");
    }
    while (1)
    {
    }
    return 0;
}
