#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <string.h>    //strlen
#include <fcntl.h>
#include <cerrno>
#include <poll.h>

#define BUFLEN 100 // Max length of buffer
#define PORT 42069 // The port on which to listen for incoming data

void die(char *s)
{
    perror(s);
    exit(1);
}

int acceptSocket;
char buf[BUFLEN];
long sentbytes;

int CreateSocket()
{
    struct sockaddr_in si_me, si_other, si_other2;

    int s, i, slen = sizeof(si_other), recv_len;

    // create a UDP socket
    if ((acceptSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    // zero out the structure
    memset((char *)&si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind socket to port
    if (bind(acceptSocket, (struct sockaddr *)&si_me, sizeof(si_me)) == -1)
    {
        die("bind");
    }
    char *message = "Hello from RDS";
    // keep listening for data
    while (1)
    {
        printf("Waiting for data...");
        fflush(stdout);

        // Send some data
        if (send(acceptSocket) message, strlen(message), 0) < 0)
            {
                puts("Send failed\n");
                return;
            }
        puts("Data Send\n");

        // try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(acceptSocket, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen)) == -1) // read datagram from server socket
        {
            die("recvfrom()");
        }

        // print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n", buf);

        XcpIp_RxCallback((uint16)rc, (uint8 *)buf, (uint16)port);

        makeTimer("First Timer", &firstTimerID, 2, 2);     // 2ms
        makeTimer("Second Timer", &secondTimerID, 10, 10); // 10ms
        makeTimer("Third Timer", &thirdTimerID, 100, 100); // 100ms
    }

    close(s);
}