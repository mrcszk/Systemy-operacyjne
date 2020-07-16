#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
 
int main(int argc, char *argv[]){
 
/*int pfd[2];
size_t nread;
char buf[100];
 
pipe(pfd);
char * str ="Ur beautiful pipe example";
write(pfd[1], str, strlen(str));
nread=read(pfd[0],buf, sizeof(buf));
 
(nread!=0)?printf("%s (%ld bytes)\n",buf,(long)nread):printf("No data\n");
*/
  int pfd[2];
  size_t nread;
  pid_t f;
  char buf[100];
  char *str = "Ur beautiful pipe example";

  /* Create the pipe. */
  if (pipe(pfd)) {
    fprintf(stderr, "Pipe failed.\n");
    return EXIT_FAILURE;
  }

  f = fork();

  if (f > 0) {
    // closing read end of the pipe
    close(pfd[0]);

    printf("PID: %d, Writing: %s\n", f, str);

    write(pfd[1], str, strlen(str));

    // closing write end of the pipe
    close(pfd[1]);
    wait(NULL);
    exit(EXIT_SUCCESS);

  } else if (f == 0) {
    // closing write end of the pipe
    close(pfd[1]);
    
    printf("PID: %d, Reading: %s\n", f, str);

    nread = read(pfd[0], buf, sizeof(buf));
    (nread != 0) ? printf("%s (%ld bytes)\n", buf, (long)nread)
                 : printf("No data\n");
    // closing read end of the pipe
    close(pfd[0]);
    exit(EXIT_SUCCESS);
  }
return 0;
}
