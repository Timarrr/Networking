#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 55000
  
int main(int argc, char const* argv[])
{
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char hello[] = {"Hello from client\n"};
    char buffer[100000] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
	std::cout << "\n Socket creation error \n";
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "192.168.1.233", &serv_addr.sin_addr) <= 0)
    {
	std::cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }
    while(1)
    {
	if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) 
	{
	    std::cout << "\nConnection Failed \n";
	}
	else break;
    }
    send(client_fd, hello, strlen(hello), 0);
    std::cout << "Hello message sent\n";
    valread = read(client_fd, buffer, 1024);
    std::cout << std::string(buffer);
    // closing the connected socket
    close(client_fd);
    return 0;
}
