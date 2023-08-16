#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT "47280"
#define FRAG_SIZE 1024

int main(int argc, char **argv) {
  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo; // will point to the results

  printf("memset()\n");
  memset(&hints, 0, sizeof hints); // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
  hints.ai_flags = AI_PASSIVE;

  // get ready to connect
  printf("getaddrinfo()\n");
  status = getaddrinfo(NULL, PORT, &hints, &servinfo);

  int sockfd;
  printf("socket()\n");
  if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
                       servinfo->ai_protocol)) < 0) {
    printf("socket() failed, exiting");
    exit(-1);
  }

  printf("bind()\n");
  if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
    printf("bind() failed, exiting");
    exit(-1);
  }

  printf("listen()\n");
  printf("listening with sockfd=%d\n", sockfd);
  if (listen(sockfd, 10)) {
    printf("listen() failed, exiting");
    exit(-1);
  }

  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  int conn_fd;
  addr_size = sizeof their_addr;
  if ((conn_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size)) <
      0) {
    printf("accept() failed, exiting");
    exit(-1);
  }
  printf("got connection\n");

  int64_t size;
  recv(conn_fd, &size, sizeof size, 0);
  int32_t name_size;
  recv(conn_fd, &name_size, sizeof name_size, 0);
  char name[name_size];
  memset(name, 0, name_size);
  int nsread = recv(conn_fd, name, name_size, 0);

  printf("filesize is %ld, name is \"%s\"(%d,%d)\n", size, name, name_size, nsread);

  FILE *fd = fopen(name, "w");
  char *buffer = malloc(FRAG_SIZE);
  int64_t recv_size = 0;
  printf("recv_size is %ld\n", recv_size);
  while (recv_size < size) {
    int read_data = recv(conn_fd, buffer, FRAG_SIZE, 0);
    recv_size += read_data;
    printf("recieved %ld b\n", recv_size);
    fwrite(buffer, sizeof buffer[0], read_data, fd);
    memset(buffer, 0, FRAG_SIZE);
  }
  printf("recieved %ld b\n", recv_size);
}