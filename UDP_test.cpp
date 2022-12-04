#include "UDP.hpp"
#include <unistd.h>

int main()
{
    UDP u1;

    u1.initUDP();

    while (1)
    {
        u1.sendUDP();
        u1.reciveUDP();
        usleep(200000);
    }

    return 0;
}