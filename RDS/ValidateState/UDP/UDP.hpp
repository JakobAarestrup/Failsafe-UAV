#define MAXLINE 1024

class UDP
{
public:
    UDP();
    ~UDP();
    void initUDP();
    void UDP_COM(char *message, char receiveMsg[]);

private:
    int socket_desc_;
    int n_;
    unsigned int *len_;
    char buffer_[MAXLINE];
    const char *IP = "192.168.0.234";
};