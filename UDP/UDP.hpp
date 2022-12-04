#define MAXLINE 1024

class UDP
{
public:
    int initUDP();
    int sendUDP();
    int  receiveUDP();
private:
    int socket_desc_;
    char *message_;
    int n_;
    unsigned int* len_;
    char buffer_[MAXLINE];
    const char* IP = "192.168.0.234";
};