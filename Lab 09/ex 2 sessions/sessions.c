#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
 
int main(void) {
 
  pid_t pid, sid;
  pid = fork();
  sid = getsid(0);

  if (pid < 0) exit(EXIT_FAILURE);
  if (pid > 0) {
    printf("Rodzic:  pid=%d ppid=%d sid=%d\n",getpid(),getppid(),sid);
  }
  if (pid == 0) {
    pid_t child_sid = setsid();
    
    printf("Dziecko: pid=%d ppid=%d sid=%d\n",getpid(),getppid(),child_sid);
  }
 
  exit(EXIT_SUCCESS);
}
