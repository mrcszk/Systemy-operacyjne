#define _GNU_SOURCE

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// ports range: 0 - 65535
#define MYPORT 3163    // the port users will be connecting to
#define BACKLOG 10      // how many pending connections queue will hold
#define MAXDATASIZE 100 // max number of bytes we can get at once

void sigchld_handler(int s) {
  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
}

int main(void) {
  int sockfd, new_fd;            // listen on sock_fd, new connection on new_fd
  struct sockaddr_in my_addr;    // my address information
  struct sockaddr_in their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int read_res, yes = 1;

  char buffer[MAXDATASIZE], buffer2[MAXDATASIZE];
  // set of socket descriptors
  fd_set read_fds;
  // iterator, temporary socket descriptor and activity flag
  int i, sd, activity;
  // max socket descriptors
  int max_sd = 0;
  // initialise all client_socket[] to 0 so not checked
  int client_socket[BACKLOG];
  for (i = 0; i < BACKLOG; i++) {
    client_socket[i] = 0;
  }

  // create a master socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  // set master socket to allow multiple connections, this is just a good habit,
  // it will work without this
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
  }

  // type of socket created
  my_addr.sin_family = AF_INET;         // host byte order
  my_addr.sin_port = htons(MYPORT);     // short, network byte order
  my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
  memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

  // bind the socket to localhost port MYPORT
  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
    perror("bind");
    exit(1);
  }
  printf("Listener on port: %d \n", MYPORT);

  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  // accept the incoming connection
  puts("Waiting for connections ...");

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  int pid_1, pid_2;
  while (1) {
    // clear the socket set
    FD_ZERO(&read_fds);

    // add master socket to set
    FD_SET(sockfd, &read_fds);
    max_sd = sockfd;

    // add child sockets to set
    for (i = 0; i < BACKLOG; i++) {
      // socket descriptor
      sd = client_socket[i];

      // if valid socket descriptor then add to read list
      if (sd > 0)
        FD_SET(sd, &read_fds);

      // highest file descriptor number, need it for the select function
      if (sd > max_sd)
        max_sd = sd;
    }

    // wait for an activity on one of the sockets , timeout is NULL ,
    // so wait indefinitely
    activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
    if ((activity < 0) && (errno != EINTR)) {
      perror("select");
    }

    // If something happened on the master socket ,
    // then its an incoming connection
    if (FD_ISSET(sockfd, &read_fds)) {
      if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr,
                           &sin_size)) == -1) {
        perror("accept");
        exit(1);
      }

      // inform user of socket number - used in send and receive commands
      printf("New connection , socket fd is %d , ip is : %s , port : %d \n ",
             new_fd, inet_ntoa(their_addr.sin_addr),
             ntohs(their_addr.sin_port));

      // add new socket to array of sockets
      for (i = 0; i < BACKLOG; i++) {
        // if position is empty
        if (client_socket[i] == 0) {
          client_socket[i] = new_fd;
          printf("Adding to list of sockets as %d\n", i);

          break;
        }
      }
    }

    // else its some IO operation on some other socket
    for (i = 0; i < BACKLOG; i++) {
      sd = client_socket[i];

      if (FD_ISSET(sd, &read_fds)) {
        // Check if it was for closing , and also read the
        // incoming message
        if ((read_res = read(sd, buffer, MAXDATASIZE)) == 0) {
          // Somebody disconnected , get his details and print
          getpeername(sd, (struct sockaddr *)&their_addr, &sin_size);
          printf("Host disconnected , ip %s , port %d \n",
                 inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port));

          // Close the socket and mark as 0 in list for reuse
          close(sd);
          client_socket[i] = 0;
        } else {
          // Establish comunication with another client
          if ((pid_1 = fork()) == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
          }
          if (pid_1 > 0) {

            if ((pid_2 = fork()) == -1) {
              perror("fork");
              exit(EXIT_FAILURE);
            }
            if (pid_2 == 0) {
              // set the string terminating NULL byte on the end
              // of the data read
              buffer[read_res] = '\0';
              printf("Socket: %d  Received: %s\n", sd, buffer);
            } else {
              scanf("%s", buffer2);
              if (send(sd, buffer2, strlen(buffer2), 0) == -1) {
                perror("send");
                exit(1);
              }
              printf("To socket: %d Answer: %s\n", sd, buffer);
            }
          }
        }
      }
    }
  }

  return 0;
}
