#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<string.h>	    //strlen

#define MAXLINE 1024 

int main(int argc , char *argv[]){
	int socket_desc;
	struct sockaddr_in server;
    char *message;
	int n, len;
	char buffer[MAXLINE]; 
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_DGRAM , 0);
	if (socket_desc == -1) printf("Could not create socket");
			
	server.sin_addr.s_addr = inet_addr("192.168.0.234");
	server.sin_family = AF_INET;
	server.sin_port = htons(20001);

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){
		puts("connect error");
		return 1;
	}
	
	puts("Connected");

    //Send some data
	message = "Hello from RDS";
	if( send(socket_desc , message , strlen(message) , 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n");

	n = recvfrom(sock_desc, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &server.sin_addr.s_addr, &len); 
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

	return 0;
}