#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define SERVER_PORT 8282
#define SERVER_IP   "192.168.3.5"
char buf[]="haoliming";
struct sockaddr_in sockaddr_server;
struct sockaddr_in sockaddr_client;
int main()
{
    int clientfd = socket(AF_INET,SOCK_STREAM,0);
    if(clientfd == -1)
    {
        fprintf(stderr,"socket error\n");
        return -1;
    }
    sockaddr_server.sin_family = AF_INET;
    sockaddr_server.sin_port = htons(SERVER_PORT);
    if(inet_aton(SERVER_IP,&sockaddr_server.sin_addr) == 0)
    {
        fprintf(stderr,"error ip address\n");
        return -2;
    }
    memset(sockaddr_server.sin_zero,0,8);
    int ret = connect(clientfd,(const sockaddr *)&sockaddr_server,sizeof(sockaddr));
    if(ret == -1)
    {
        fprintf(stderr,"connect failed\n");
        return -3;
    }
    while(1)
    {
        int size = send(clientfd,buf,sizeof(buf),0);
        if(size <= 0)
        {
            fprintf(stderr,"send error");
            close(clientfd);
            return -1;
        }
        sleep(2);
    }

}
