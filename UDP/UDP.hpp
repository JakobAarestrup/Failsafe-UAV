#include<string>	    //strlen
#include<iostream>
#define MAXLINE 1024

class UDP
{
public:
    void initUDP();
    void sendUDP();
    int  receiveUDP();
private:
    int socket_desc_;
    char *message_;
    int n_;
    unsigned int* len_;
    char buffer_[MAXLINE];
    std::string IP = "192.168.0.234";
};