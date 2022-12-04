#include "UDP.hpp"
#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<string.h>		//strlen	

#define MAXLINE 1024
	
void UDP::initUDP()
{
	struct sockaddr_in server;
	//Create socket
	socket_desc_ = socket(AF_INET , SOCK_DGRAM , 0);
	if (socket_desc_ == -1) printf("Could not create socket");
			
	server.sin_addr.s_addr = inet_addr(IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(20001);

	//Connect to remote server
	if (connect(socket_desc_ , (struct sockaddr *)&server , sizeof(server)) < 0){
		puts("connect error");
		return 1;
	}
	puts("Connected");
}

void UDP::sendUDP()
{
	//Send some data
	message_ = "Hello from RDS";
	if( send(socket_desc_ , message_ , strlen(message_) , 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n");
}

int UDP::receiveUDP()
{
	n_ = recvfrom(socket_desc_, (char *)buffer_, MAXLINE, MSG_WAITALL, (struct sockaddr *) &server, len_); 
    buffer_[n] = '\0';
    printf("Server: %s\n", buffer_);
}