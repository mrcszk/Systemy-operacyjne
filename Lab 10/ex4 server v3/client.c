//lab 12 -> Dla zainteresowanych -> Komunikator - transfer plików
// zapisujemy plik wysłany przez serwer
// gcc -o client client.c; ./client NAZWA_URZĄDZENIA NAZWA_PLIKU

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>
  
#define PORT 3163 // the port client will be connecting to      
#define MAXDATASIZE 100 // max number of bytes we can get at once 
     
int main(int argc, char *argv[])
{
  int sockfd, numbytes;  
  char buf[MAXDATASIZE];
  struct hostent *he;
  struct sockaddr_in their_addr; // connector's address information 

  FILE* rcv;
  int file_size;
  int len;

  if (argc != 3) {
    fprintf(stderr,"usage: client hostname, file name\n");
    exit(1);
  }
     
  if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info 
    perror("gethostbyname");
    exit(1);
  }
     
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }
     
  their_addr.sin_family = AF_INET;    // host byte order 
  their_addr.sin_port = htons(PORT);  // short, network byte order 
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);
     
  if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof their_addr) == -1) {
    perror("connect");
    exit(1);
  }
     
  if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
    perror("recv");
    exit(1);
  }

// odbieranie pliku
  file_size = atoi(buf);   
  buf[numbytes] = '\0';
     
  printf("Received: %s\n",buf);

  sleep(1);

  rcv = fopen (argv[2], "w");

  while (((len = recv(sockfd, buf, BUFSIZ, 0)) > 0))
    {      
      fwrite(buf, sizeof(char), len, rcv);
      printf("  writing\n");
    }
  printf("finish\n");

  if (fclose(rcv)) { perror("fclose"); exit(-1); }
  if (close(sockfd)) { perror("close"); exit(-1); }
     
  return 0;
}

