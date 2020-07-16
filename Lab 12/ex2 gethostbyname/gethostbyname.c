#include <errno.h>
#include <netdb.h>      /* gethostbyname, hostent structure */
#include <netinet/in.h> /* ntohn */
#include <stdio.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
 
#define h_addr h_addr_list[0] // for backward compatibility

int main(int argc, char *argv[])
{
     int i,j = 0;
     struct hostent *he;
 
     if (argc != 2) 
     {
          fprintf(stderr,"usage: %s hostname\n", argv[0]);
          return 1;
     }
 
      if ((he = gethostbyname(argv[1])) == NULL) {
        printf("gethostbyname: %s\n", hstrerror(h_errno));
        return 1;
      }
 
     /* host info: */
     printf("\nHost name: %s", he->h_name);
     printf("\nAliases:");
     for(i=0;he->h_aliases[i] != NULL;++i)
          printf("\n%d. %s", i+1, he->h_aliases[i]);
     if(he->h_addrtype == AF_INET)
          printf("\nAddres type: IPv4");
     if(he->h_addrtype == AF_INET6)
          printf("\nAddres type: IPv6");
     printf("\nAddress length: %d bytes", he->h_length);
     printf("\nAddresses:");
     int counter = 1;
     while (he->h_addr_list[j] != NULL) {
         if (counter > 1) {
           printf(", ");
         }
         printf("%s ", inet_ntoa(*(struct in_addr *)(he->h_addr_list[j])));
         j++;
         counter++;
     }
     printf("\n");
 
     return 0;
}
