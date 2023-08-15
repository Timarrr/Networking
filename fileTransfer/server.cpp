#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server.hpp"
#define PORT 55000
int main(int argc, const char **argv)
{
//	getFile(argc, argv);
    int server_fd, clientSocket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
//    char buffer[1024] = { 0 };
	
    char hello[] = {"Hello from server\n"};
  
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
	std::cerr << "socket failed\n";
        return -1;
    }
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    {
	std::cerr << "setsockopt\n";
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
  
    // Forcefully attaching socket to the port 
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) 
    {
	std::cerr << "bind failed\n";
    	return -2;
    }
    if(listen(server_fd, 3) < 0) 
    {
	std::cerr << "listen\n";
   	return -3;
    }
	std::cout << "initializing client socket ...\n";
    clientSocket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if(clientSocket == -1)
    {
        std::cerr << "Problem with client connecting!";
        return -4;
    }
	std::cout << "connection established\n";
//    std::cout << "Client address: " << inet_ntoa(client.sin_addr) << " and port: " << client.sin_port << std::endl;

    close(server_fd);

    char buf[4096];
    std::string content_ = getFile(argc, argv);
    std::string filename = argv[1];
    std::string size_ = std::to_string(content_.size());
    std::string fuckYouString = "BEGINNING\n" + filename + '\n' + size_ + '\n' + content_;
    std::cout << "listening ...\n";
    uint32_t actualSize = fuckYouString.size();
    uint32_t bytesSent = 0;
    while (true)
    {
        // clear buffer
        memset(buf, 0, 4096);
        // wait for a message
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1)
        {
            std::cerr << "There was a connection issue." << std::endl;
        }
        if (bytesRecv == 0)
        {
            std::cout << "The client disconnected" << std::endl;
        }
        // display message
        std::cout << "Received: " << std::string(buf, 0, bytesRecv);
        // return message
        bytesSent += send(clientSocket, fuckYouString.c_str(), fuckYouString.size(), 0);
    }
    // close socket
    close(clientSocket);

//    valread = read(new_socket, buf, 4096);
//    std::cout << std::string(buffer);
//    send(new_socket, hello, strlen(hello), 0);
//    std::cout << "Hello message sent\n";
    // closing the connected socket
//    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}
