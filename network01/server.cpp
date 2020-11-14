#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>



int main()
{
	int listenfd;
    struct sockaddr_in sockaddr_server;
    struct sockaddr_in sockaddr_client;
    char server_ip[]="192.168.3.5";
    char bufs[128];
    uint16_t server_port = 8282;
    unsigned int addr_len;
    int client_sockfd;
    int len = 0;
    printf("new start\n");
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd == -1)
    {
        fprintf(stderr,"socket error\n");
        return -1;
    }
    printf("socket:%d\n",listenfd);
    sockaddr_server.sin_family = AF_INET;
    sockaddr_server.sin_port = htons(server_port);
    if(inet_aton(server_ip,&sockaddr_server.sin_addr) == 0)
    {
        fprintf(stderr,"error IP Address\n");
        return -2;
    }
    memset(sockaddr_server.sin_zero,0,8);
    int ret = bind(listenfd,(const struct sockaddr*)&sockaddr_server,sizeof(struct sockaddr));
    if(ret == -1)
    {
        fprintf(stderr,"bind error\n");
        return -3;
    }

    ret = listen(listenfd,10);
    if(ret == -1)
    {
        fprintf(stderr,"listen error\n");
        return -3;
    }
    printf("server init ok\n");
    addr_len = sizeof(struct sockaddr);
    
	while(1)
	{
		// 父进程负责接待新连接，然后创建新进程处理新连接的请求，
		// 每当有新连接接入就给它分配一个新进程
		client_sockfd = accept(listenfd,(struct sockaddr*)&sockaddr_client,&addr_len);//等待新连接
		if(client_sockfd != -1)
		{
			printf("new connect ip:%s\n",inet_ntoa(sockaddr_client.sin_addr));
		}
		if(!fork())
		{
			close(listenfd);//子进程关闭监听socket
			for(;;)//子进程处理新连接的请求
			{
				memset(bufs,0,128);
				int len = recv(client_sockfd,bufs,sizeof(bufs)+1,0);
				bufs[len]='\0';
				if(len <= 0)
				{
					close(client_sockfd);
					printf("recv error\n");
					return -1;
				}
				else
				{
					printf("from %s:%d :%s\n",inet_ntoa(sockaddr_client.sin_addr),sockaddr_client.sin_port,bufs);
				}
			}		
		}
		close(client_sockfd);//父进程关闭连接套接字,这个连接已经交给子进程处理　
		
		
	}
	close(listenfd);
	return 0;
}
