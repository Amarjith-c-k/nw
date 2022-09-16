#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main()
{
  int sockfd;
  char buffer[256];
  struct sockaddr_in serv_addr;
  int n;
  sockfd = socket(AF_INET,SOCK_STREAM,0);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(3000);
  serv_addr.sin_addr.s_addr=INADDR_ANY;
  connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
  printf("Please enter the message: \n");
  scanf("%s",buffer);
  write(sockfd,buffer,strlen(buffer));
}
