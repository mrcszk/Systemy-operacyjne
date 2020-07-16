#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int BEGIN = 1;
int END = 10;

int main(int argc, char *argv[]) {
  pid_t child_a, child_b;
  int first_pipe[2], second_pipe[2];

  /* create the pipes */
  if (pipe(first_pipe) == -1 || pipe(second_pipe) == -1) {
    fprintf(stderr, "Pipe failed.\n");
    return EXIT_FAILURE;
  }

  child_a = fork();

  /* fork error handling */
  if (child_a < 0) {
    fprintf(stderr, "Fork failed");
    return EXIT_FAILURE;
  }

  if (child_a == 0) {
    /* child A code */

    close(first_pipe[0]);

    int i;
    for (i = BEGIN; i <= END; i++) {
      write(first_pipe[1], &i, sizeof(i));
    }
    close(first_pipe[1]);

  } else {
    child_b = fork();

    if (child_b < 0) {
      fprintf(stderr, "Fork failed");
      return EXIT_FAILURE;
    }

    if (child_b == 0) {
      /* Child B code */

      close(first_pipe[1]);
      close(second_pipe[0]);

      int num;
      while (1) {
        /* if the parent closes the write end of the first pipe, break out of
         * the loop. */
        if (read(first_pipe[0], &num, sizeof(num)) > 0) {
          // printf("Child B read value: %d\n", num);
          num *= 5;
          // printf("Child B write value: %d\n", num);
          write(second_pipe[1], &num, sizeof(num));
        } else {
          break;
        }
      }

      close(first_pipe[0]);

    } else {
      /* Parent Code */

      close(first_pipe[0]);
      close(first_pipe[1]);
      close(second_pipe[1]);

      printf("Received numbers:\n");
      int num;
      while (1) {
        /* if the parent closes the write end of the first pipe, break out of
         * the loop. */
        if (read(second_pipe[0], &num, sizeof(num)) > 0) {
          printf("%d\n", num);
          num *= 5;
        } else {
          break;
        }
      }

      close(second_pipe[0]);
    }
  }

  return 0;
}
