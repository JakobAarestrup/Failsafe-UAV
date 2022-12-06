#define MAXLINE 1024

class UDP
{
public:
    UDP();
    ~UDP();
    void initUDP();
    void UDP_COM(const char *message);

private:
    int socket_desc_;
    int n_;
    unsigned int *len_;
    char buffer_[MAXLINE];
    const char *IP = "172.20.10.3";
};