#include "UDP.hpp"
#include <unistd.h>
#include<stdio.h>

int main()
{
    UDP u1;
    u1.initUDP();
    char *message = "Hello from RDS";
    char receivedMsg[];

    while (1)
    {
        u1.sendUDP(message);
        strcpy(receivedMsg,u1.receiveUDP());
        printf("Server: %s\n",receivedMsg);
        usleep(200000);
    }

    return 0;
}