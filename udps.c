
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int udpSocket, nBytes;
    char buffer[1024];
    struct sockaddr_in serverAddr, clientAddr;
    // struct sockaddr_storage serverStorage;
    socklen_t addr_size, client_addr_size;
    int i;

    /*Create UDP socket*/
    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    /*Bind socket with address struct*/
    bind(udpSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    /*Initialize size variable to be used later on*/
    addr_size = sizeof(serverAddr);

    while (1)
    {
        /* Try to receive any incoming UDP datagram. */
        nBytes = recvfrom(udpSocket, buffer, 1024, 0, (struct sockaddr *)&serverAddr, &addr_size);
        printf("%s", buffer);

        printf("enter the message to client");
        fgets(buffer, 1024, stdin);

        sendto(udpSocket, buffer, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size);
    }

    return 0;
}
