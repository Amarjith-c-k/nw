#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct frame
{

    int sq_no;
    int ack;
} Frame;
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <port>", argv[0]);
        exit(0);
    }
    int udpSocket, nBytes;
    int port = atoi(argv[1]);
    char buffer[1024];
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size, client_addr_size;
    int i, nf, flag, k = 0;
    srand(time(NULL));
    Frame frec;

    /*Create UDP socket*/
    if ((udpSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("error in socket creation");
        exit(0);
    }
    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    /*Bind socket with address struct*/
    if ((bind(udpSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) < 0)
    {
        perror("bind");
        exit(0);
    }

    /*Initialize size variable to be used later on*/
    addr_size = sizeof(serverAddr);
    recvfrom(udpSocket, buffer, 1024, 0, (struct sockaddr *)&serverAddr, &addr_size);
    nf = buffer[0] - 48;
    while (k < nf)
    {
        recvfrom(udpSocket, &frec, sizeof(Frame), 0, (struct sockaddr *)&serverAddr, &addr_size);
        flag = rand() % 2;
        if (!flag)
        {
            printf("\nFrame %d recieved\n", frec.sq_no);
            frec.ack = frec.sq_no;
            sendto(udpSocket, &frec, sizeof(frec), 0, (struct sockaddr *)&serverAddr, addr_size);
            k++;
        }
        else
        {
            printf("\nFrame %d not recieved", frec.sq_no);
            printf("\n send negative acknowledgement\n retransmit the window");
            frec.ack = -frec.sq_no;
            sendto(udpSocket, &frec, sizeof(frec), 0, (struct sockaddr *)&serverAddr, addr_size);
        }
    }
    return 0;
}
