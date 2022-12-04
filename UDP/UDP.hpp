#define MAXLINE 1024

class UDP
{
public:
    int initUDP();
    int sendUDP(char* message);
    void receiveUDP();
    void getUDP(char receiveMsg[]);
private:
    int socket_desc_;
    int n_;
    unsigned int* len_;
    char buffer_[MAXLINE];
    const char* IP = "192.168.0.234";
};