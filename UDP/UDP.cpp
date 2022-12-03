#include <stdio.h>
#include <sys/socket.h>

//Socket() creates a socket and return descriptor. 
//AF_INET = adress family (IPV4). 
//SOCK_DGRAM = Decides protocol (TCP/UDP) = UDP. 
//0 = IP protocol

int main(int argc, char &argv[]){
    int socket_desc;
    socket_desc = socket(AF_INET, SOCK_DGRAM, 0); 
    if (socket_desc == -1) printf("Could not create socket.");
    return 0;

    struct sockaddr_in server;

    struct sockaddr_in 
    {
        short            sin_family;   // e.g. AF_INET, AF_INET6
        unsigned short   sin_port;     // e.g. htons(3490)
        struct in_addr   sin_addr;     // see struct in_addr, below
        char             sin_zero[8];  // zero this if you want to
    };

    struct in_addr 
    {
        unsigned long s_addr;          // load with inet_pton()
    };

    struct sockaddr 
    {
        unsigned short    sa_family;    // address family, AF_xxx
        char              sa_data[14];  // 14 bytes of protocol address
    };

    server.sin_addr.s_addr = inet_addr("74.125.235.20"); // IP is the address of google.com

    
	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){
		puts("connect error");
		return 1;
	}
	
	puts("Connected");
	return 0;
}