#include "UDP.hpp"
#include <unistd.h>
#include<stdio.h>
#include<string>

int main()
{
    UDP u1;
    u1.initUDP();
    char *message = "Hello from RDS";
    char receivedMsg[1024];

    while (1)
    {
        u1.UDP_COM(message, receivedMsg);
        //u1.receiveUDP(receivedMsg);
        printf("Server: %s\n",receivedMsg);
        usleep(200000);
    }

    return 0;
}