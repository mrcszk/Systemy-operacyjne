#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
 
int main(void)
{
    int     pfd[2];
    pid_t   pid;
    char    string[] = "Test";
    char    buf[10];
 
    if (pipe(pfd) == -1) {
        fprintf(stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    pid = fork();

    if(pid == 0) {
        /*close(0);              
	    dup(pfd[0]);*/        
        close(pfd[1]);

        dup2(pfd[0], 0);
        read(STDIN_FILENO, buf, sizeof(buf));
        close(pfd[0]);
        printf("Wypisuje: %s\n", buf);        
        exit(0);    
    } 
    else {
        close(pfd[0]);
        write(pfd[1], string, (strlen(string) + 1));
        close(pfd[1]);
        wait(NULL);        
    }
 
    return 0;
}
