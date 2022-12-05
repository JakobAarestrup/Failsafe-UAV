#include "UDP.hpp"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <string.h>	   //strlen

#define MAXLINE 1024
struct sockaddr_in server;

int UDP::initUDP()
{

	// Create socket
	socket_desc_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_desc_ == -1)
		printf("Could not create socket");

	server.sin_addr.s_addr = inet_addr(IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(42069);

	// Connect to remote server
	if (connect(socket_desc_, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	puts("Connected");
	return 0;
}

void UDP::UDP_COM(char *message, char receiveMsg[])
{
	// Send some data
	if (send(socket_desc_, message, strlen(message), 0) < 0)
	{
		puts("Send failed");
		return;
	}
	puts("Data Send\n");

	// Receive message
	if ((n_ = recvfrom(socket_desc_, (char *)buffer_, MAXLINE, MSG_WAITALL, (struct sockaddr *)&server, len_)) <= 0)
	{
		puts("Receive failed");
		return;
	}

	buffer_[n_] = '\0';

	int strLength = strlen(buffer_); // finds length of the array
	for (int i = 0; i < strLength; i++)
	{
		receiveMsg[i] = buffer_[i]; // copies UserInput in reverse to TempInput
	}
	receiveMsg[strLength] = '\0'; // adds NULL character at end
}
