#include "UDP.hpp"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <string.h>	   //strlen

/// Struct with settings for UDP server
struct sockaddr_in server;

/**
 * @brief Construct a new UDP::UDP object
 *
 */
UDP::UDP()
{
}

/**
 * @brief Destroy the UDP::UDP object
 *
 */
UDP::~UDP()
{
}

/**
 * @brief Function initializes the UDP connection and connects to server
 *
 */
void UDP::initUDP()
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
		return;
	}
	puts("Connected");
}

/**
 * @brief Function sends data to UDP server
 *
 * @param message - message to send
 */
void UDP::UDP_COM(const char *message)
{
	// Send data
	if (send(socket_desc_, message, strlen(message), 0) < 0)
	{
		puts("Send failed");
		return;
	}
	// puts("Data Send\n");
}
