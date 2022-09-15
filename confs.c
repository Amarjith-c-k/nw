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
// function sending file
int sendFile(FILE *fp, char *buf, int s)
{
    int i, len;

    char ch, ch2;
    for (i = 0; i < s; i++)
    {
        ch = fgetc(fp);

        buf[i] = ch;
        if (ch == EOF)
            return 1;
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
    addr_con.sin_addr.s_addr = INADDR_ANY;
    char net_buf[1024];
    pid_t cp;
    char str[20] = "File not found";
    FILE *fp;

    // socket()
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
        printf("\nfile descriptor not received!!\n");
    else
        printf("\nfile descriptor %d received\n", sockfd);

    // bind()
    if (bind(sockfd, (struct sockaddr *)&addr_con, sizeof(addr_con)) == 0)
        printf("\nSuccessfully binded!\n");
    else
        printf("\nBinding Failed!\n");

    while (1)
    {

        printf("\nWaiting for file name...\n");

        // receive file name
        clearBuf(net_buf);

        nBytes = recvfrom(sockfd, net_buf,
                          1024, 0,
                          (struct sockaddr *)&addr_con, &addrlen);
        printf("\nFile Name Received: %s\n", net_buf);
        fp = fopen(net_buf, "r");
        if ((cp = fork()) == 0)
        {

            if (fp == NULL)
            {
                clearBuf(net_buf);
                printf("\nFile open failed!\n");
                strcpy(net_buf, str);
                sendto(sockfd, net_buf, 1024, 0, (struct sockaddr *)&addr_con, addrlen);
                clearBuf(net_buf);
            }
            else
            {
                printf("\nFile Successfully opened!\n");
                // process

                sendFile(fp, net_buf, 1024);
                sendto(sockfd, net_buf, 1024, 0, (struct sockaddr *)&addr_con, addrlen);
                clearBuf(net_buf);
            }

            // fclose(fp);
        }
        else if (cp < 0)
        {
            perror("fork error");
            exit(0);
        }
    }
    return 0;
}