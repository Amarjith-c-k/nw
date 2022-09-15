#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
// function to clear buffer
void clearBuf(char *b)
{
    int i;
    for (i = 0; i < 1024; i++)
        b[i] = '\0';
}

// function to receive file
int recvFile(char *buf, int s)
{
    int i;
    char ch;
    for (i = 0; i < s; i++)
    {

        ch = buf[i];
        if (ch == EOF)
            return 1;
        else
            printf("%c", ch);
    }
    return 0;
}

// driver code
int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Usage: %s <port>", argv[0]);
        exit(0);
    }
    int port = atoi(argv[1]);
    int sockfd, nBytes;
    struct sockaddr_in addr_con;
    int addrlen = sizeof(addr_con);
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(port);
    addr_con.sin_addr.s_addr = inet_addr("127.0.0.1");
    char net_buf[1024];
    FILE *fp;

    // socket()
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
        printf("\nfile descriptor not received!!\n");
    else
        printf("\nfile descriptor %d received\n", sockfd);

    while (1)
    {
        printf("\nPlease enter file name to receive:\n");
        scanf("%s", net_buf);
        sendto(sockfd, net_buf, 1024, 0, (struct sockaddr *)&addr_con,
               addrlen);

        // receive
        clearBuf(net_buf);
        nBytes = recvfrom(sockfd, net_buf, 1024, 0, (struct sockaddr *)&addr_con,
                          &addrlen);

        recvFile(net_buf, nBytes);
    }
    printf("\n-------------------------------\n");

    return 0;
}
