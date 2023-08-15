#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 55000
std::string filename_g = "";
void writeFile(char* array_ch)
{
    std::stringstream code{array_ch};
    std::string previousContent{};

    std::string firstLine = "";
    std::string filename_ = "";
    std::string size_ = "";
    std::getline(code, firstLine);
    bool new_f = 1;
    if(firstLine == "BEGINNING")
    {
	std::getline(code, filename_, '\n');
	filename_g = filename_;
	std::getline(code, size_, '\n');
    }
    else
    {
	filename_ = filename_g;
	std::string line{};
	std::ifstream sameFuckingFile(filename_, std::ios::binary);
	while(std::getline(sameFuckingFile, line, '\0'))	
	{
	    sameFuckingFile >> line;
	    previousContent += line +'\n';
	}
	sameFuckingFile.close();
    }
    std::ofstream file(filename_, std::ios::binary);
    if(!new_f)	
    {
	file << previousContent << '\n';
	file << firstLine << '\n';
    }
    std::string line;
    while(std::getline(code, line, '\0'))	file << line << '\n';
    file.close();
    
}
int main(int argc, char const* argv[])
{
    std::string ip = (argc > 1) ? argv[1] : "192.168.1.233";//works?	
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char hello[] = {"Hello from client\n"};
    char buffer[65535] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
	std::cout << "\n Socket creation error \n";
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0)
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
    while(1)
    {
	valread = read(client_fd, buffer, 65535);
    	if(valread > 0)
	{
		writeFile(buffer);
		std::cout << std::string(buffer);
	}

    }
   
    // closing the connected socket
    close(client_fd);
    shutdown(client_fd, SHUT_RDWR);

    return 0;
}
