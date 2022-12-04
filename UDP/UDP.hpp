class UDP

#include<string.h>	    //strlen
#define MAXLINE 1024

{
public:
    void initUDP();
    void sendUDP();
    int  recieveUDP();
private:
    int socket_desc_;
    struct sockaddr_in server_;
    char *message_;
    int n_;
    unsigned int* len_;
    char buffer_[MAXLINE];
    string IP = "192.168.0.234";
};