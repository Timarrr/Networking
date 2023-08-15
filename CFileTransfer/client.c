#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#define SIZE 65535
 
void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[SIZE] = {0};
    while(fgets(data, SIZE, fp) != NULL)
    {
	if(send(sockfd, data, sizeof(data), 0) == -1)
	{
	    perror("[-]Error in sending file.");
	    exit(1);
	}
	bzero(data, SIZE);
    }
}
 
int main(int argc, char **argv)
{
    char *ip = argv[2];
    int port = 55000;
    int e;
    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename = argv[1];
 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
	perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
 
    e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e == -1) 
    {
	perror("[-]Error in socket");
	exit(1);
    }
    printf("[+]Connected to Server.\n");
 
    fp = fopen(filename, "r");
    if(fp == NULL)
    {
	perror("[-]Error in reading file.");
	exit(1);
    }
 
    send_file(fp, sockfd);
    printf("[+]File data sent successfully.\n");
    printf("[+]Closing the connection.\n");
    close(sockfd);
 
    return 0;
}
