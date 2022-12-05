#include "UDP.hpp"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <string.h>	   //strlen
#include <fcntl.h>
#include <cerrno>

#define MAXLINE 1024
struct sockaddr_in server;
unsigned long int noBlock; // Non-blocking flag

int UDP::initUDP()
{

	// Create socket
	socket_desc_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_desc_ == -1)
		printf("Could not create socket");

	int flags = fcntl(socket_desc_, F_GETFL, 0);
	fcntl(socket_desc_, F_SETFL, flags | O_NONBLOCK);
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

	// bind
	if (bind(socket_desc_, (sockaddr *)&server, sizeof(server)) == -1)
	{
		printf("Bind failed");
		return 1;
	}
	puts("Bind done.");
}

void UDP::UDP_COM(char *message, char receiveMsg[])
{
	// Send some data
	if (send(socket_desc_, message, strlen(message), 0) < 0)
	{
		puts("Send failed\n");
		return;
	}
	puts("Data Send\n");
	// Receive message

	struct pollfd pfd = {.socket_desc_ = s, .events = POLLIN};
	poll(&pfd, 1, 50);
	if ((n_ = recvfrom(socket_desc_, (char *)buffer_, MAXLINE, MSG_WAITALL, (struct sockaddr *)&server, len_)) < 0)
	{
		fprintf(stdout, "Unable to receive: %s\n", strerror(errno)); // error handling
		return;
	}

	buffer_[n_] = '\0';

	int strLength = strlen(buffer_); // finds length of the array
	for (int i = 0; i < strLength; i++)
	{
		receiveMsg[i] = buffer_[i]; // copies UserInput in reverse to TempInput
	}
	receiveMsg[strLength] = '\0'; // adds NULL character at end */
}
