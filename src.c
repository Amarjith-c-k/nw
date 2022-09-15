#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
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
    int clientSocket, portNum, nBytes, nf;
    char buffer[1024];
    int port = atoi(argv[1]);
    int i = 1;
    int n, j = 1, k = 0, m = 0;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    Frame fsend;

    /*Create UDP socket*/
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverAddr;

    printf("enter the number of frames");
    scanf("%s", buffer);
    nf = buffer[0] - 48;
    nBytes = strlen(buffer) + 1;
    sendto(clientSocket, buffer, 1024, 0, (struct sockaddr *)&serverAddr, addr_size);
    printf("enter the window size");
    scanf("%s", buffer);
    n = buffer[0] - 48;
    while (i <= n)
    {
        fsend.sq_no = i;
        printf("\nFrame %d send", i);
        sendto(clientSocket, &fsend, sizeof(Frame), 0, (struct sockaddr *)&serverAddr, addr_size);

        i++;
        j++;
    }
    i = 1;
    fsend.sq_no = i;
    while (k < nf)
    {
        recvfrom(clientSocket, &fsend, sizeof(fsend), 0, (struct sockaddr *)&serverAddr, &addr_size);
        printf("\nwaiting for the acknowledgement");
        if (fsend.ack > 0)
        {

            i = fsend.sq_no;
            printf("\nack %d recieved\n", fsend.ack);
            if (j <= nf)
            {
                printf("\nframe %d sent", j);
                fsend.sq_no = j;
                sendto(clientSocket, &fsend, sizeof(Frame), 0, (struct sockaddr *)&serverAddr, addr_size);
                j++;
            }
            i++;
            k++;
        }
        else
        {
            printf("\nack %d not recieved", -fsend.ack);
            printf("\nRetransmitting window");
            fsend.sq_no = -fsend.ack;
            i = fsend.sq_no;
            printf("\nFrame %d send", i);
            fsend.sq_no = i;
            sendto(clientSocket, &fsend, sizeof(Frame), 0, (struct sockaddr *)&serverAddr, addr_size);
        }
    }
    return 0;
}
