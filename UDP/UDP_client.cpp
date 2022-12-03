#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<string.h>	    //strlen

int main(int argc , char *argv[]){
	int socket_desc;
	struct sockaddr_in server;
    char *message;
	double TestData = 15.93;
	
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
	if( send(socket_desc , TestData , sizeof(TestData) , 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n");


	return 0;
}