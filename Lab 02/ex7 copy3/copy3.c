#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
     
#define BUFSIZE 512
#include "t.h"
void copy3(char *from, char *to)
{
	FILE *stfrom, *stto;
	int c;
 
	if ((stfrom = fopen(from, "r") ) == NULL){}
	if(( stto = fopen(to, "w") ) == NULL) {}
	while ((c = getc(stfrom)) != EOF)
		putc(c, stto);
	fclose(stfrom);
	fclose(stto);
	return;
 
}

ssize_t readall(int fd, void *buf, size_t nbyte){
         ssize_t nread = 0, n;
         do {
	         if ((n = read(fd, &((char *)buf)[nread], nbyte - nread)) == -1) {
		         if (errno == EINTR)
				        continue;
		         else
				        exit(EXIT_FAILURE);
	         }
	         if (n == 0)
		         return nread;    // jeżeli read pomyślnie zakończył działanie
	         nread += n;
	     } while (nread < nbyte);
     
     return nread;
    }

    ssize_t writeall(int fd, const void *buf, size_t nbyte){
	    ssize_t nwritten = 0, n;
 
	    do {
		    if ((n = write(fd, &((const char *)buf)[nwritten], nbyte - nwritten)) == -1) {
			    if (errno == EINTR)
			    	continue;
			    else
			    	return -1;
		    }
		    nwritten += n;
	    } while (nwritten < nbyte);

	    return nwritten;
    }
     
    void copy(char *from, char *to){
    	int fromfd = -1, tofd = -1;
    	ssize_t nread;
    	char buf[BUFSIZE];
        
        fromfd = open(from, O_RDONLY);

        if(fromfd == -1){
            if(errno == EACCES){    // flaga bledu Access Error permission denied
                perror("ERROR: Permission Denied!");
            } else if(errno == ENOENT){     // Error NO ENTity
                perror("ERROR: Source file does not exist!");
            } else {
                perror("ERROR");
                exit(EXIT_FAILURE);
            }
        }
        
    	tofd = open(to, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

        if (tofd == -1) {
		    perror("Error");
		    exit(errno);
	    }

    	while ((nread = read(fromfd, buf, sizeof(buf))) > 0){
    	    if (writeall(tofd, buf, nread) == -1){
                perror("ERROR: Failed writing to target file!");
                exit(EXIT_FAILURE);
            }	
        }

        if (nread == -1){
            perror("ERROR: Failed reading source file!");
            exit(EXIT_FAILURE);
        }

        if (close(fromfd) == -1 || close(tofd) == -1){
            perror("ERROR: Failed closing files!");
            exit(EXIT_FAILURE);
        }
    	return;
    }
 
int main(int argc, char** argv){
    timestart();
    copy(argv[1], argv[2]);
    timestop("Copy2");

	timestart();
    copy3(argv[1], argv[2]);
    timestop("Copy3");
	return 0;
}
