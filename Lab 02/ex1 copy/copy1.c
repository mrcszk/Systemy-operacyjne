/*
     * copy1.c
     * 
     * based on: 
     *
     * Miscellaneous examples for Chap. 2
     * AUP2, Chap. 2
     *
     * modified by WTA
     */
          
    #include <fcntl.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <errno.h>
    #include <stdlib.h>
     
    #define BUFSIZE 512
     
    void copy(char *from, char *to) 
    {
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
    	    if (write(tofd, buf, nread) == -1){
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
     
    int main(int argc, char **argv){
    	if (argc != 3)
    	{
    		fprintf(stderr,"usage: %s from_pathname to_pathname\n", argv[0]);
    		return 1;
    	}
    	copy(argv[1], argv[2]);
    	return 0;
    }
