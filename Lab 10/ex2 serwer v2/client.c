#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define h_addr h_addr_list[0] // for backward compatibility
#define PORT 3163            // the port client will be connecting to
#define MAXDATASIZE 100       // max number of bytes we can get at once

int main(int argc, char *argv[]) {
  int sockfd;
  struct hostent *he;
  struct sockaddr_in their_addr; // connector's address information

  if (argc != 2) {
    fprintf(stderr, "usage: ./client hostname\n");
    exit(1);
  }

  if ((he = gethostbyname(argv[1])) == NULL) { // get the host info
    perror("gethostbyname");
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  their_addr.sin_family = AF_INET;   // host byte order
  their_addr.sin_port = htons(PORT); // short, network byte order
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

  if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof their_addr) ==
      -1) {
    perror("connect");
    exit(1);
  }

  int pid, read_res = 0;
  char msg_to_send[MAXDATASIZE], received_msg[MAXDATASIZE];

  if ((pid = fork()) == -1)
    perror("fork");

  if (pid == 0) {
    while (1) {
      scanf("%s", msg_to_send);
      if (send(sockfd, msg_to_send, MAXDATASIZE - 1, 0) == -1)
        perror("send");
      printf("Send: %s\n", msg_to_send);
    }
  } else if (pid > 0) {
    while (1) {
      if ((read_res = read(sockfd, received_msg, MAXDATASIZE - 1)) == -1)
        perror("read");
      if (read_res > 0)
        printf("Received: %s\n", received_msg);
    }
  }

  close(sockfd);
  return 0;
}
