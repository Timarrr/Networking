#include <errno.h>
#include <netdb.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT "47280"
#define FRAG_SIZE 1024

int main(int argc, char **argv) {
  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo; // will point to the results

  memset(&hints, 0, sizeof hints); // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

  // get ready to connect
  status = getaddrinfo(argv[1], PORT, &hints, &servinfo);

  int sockfd;
  if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
                       servinfo->ai_protocol)) < 0) {
    printf("socket() failed with %d, exiting", errno);
    exit(-1);
  }

  if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
    printf("connect() failed with %d, exiting", errno);
    exit(-1);
  }

  FILE *fd = fopen(argv[2], "r");

  if (errno != 0) {
    printf("fopen() failed with %d, exiting", errno);
  }

  fseek(fd, 0L, SEEK_END);
  int64_t filesize = ftell(fd);
  fseek(fd, 0L, SEEK_SET);

  size_t remaining_bytes = filesize;

  send(sockfd, &filesize, sizeof(int64_t), 0);

  char *buffer = malloc(FRAG_SIZE);

  char *last_slash = strrchr(argv[2], '/');
  char *shortname = last_slash ? last_slash + 1 : argv[2];

  int32_t shortname_len = strlen(shortname) + 1;

  send(sockfd, &shortname_len, sizeof shortname_len, 0);
  send(sockfd, shortname, shortname_len, 0);

  int sent_size = 0;
  printf("sending %s(%d), size %ld\n", shortname, shortname_len, filesize);

  while (sent_size < filesize) {
    memset(buffer, 0, FRAG_SIZE);
    int data_read = fread(buffer, sizeof buffer[0], FRAG_SIZE, fd);

    sent_size += data_read;
    printf("read %d b\n", data_read);
    if (data_read == FRAG_SIZE) {
      // all ok
      printf("sent %ld b\n", send(sockfd, buffer, data_read, 0));

    } else {
      if (feof(fd)) {

        printf("sent %ld b\n", send(sockfd, buffer, data_read, 0));
        printf("sent %ld b\n", filesize);
        exit(0);

      } else if (ferror(fd)) {

        perror("Error reading test.bin");
        exit(-1);
      }
    }
  }
}