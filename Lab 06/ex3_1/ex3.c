#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char word[256], buf[256];
  char *pattern = "pipe";

  pid_t child_a, child_b;
  int pipe_from_parent_1[2], pipe_from_parent_2[2], pipe_from_child_a[2],
      pipe_from_child_b[2];

  /* create the pipes */
  if (pipe(pipe_from_parent_1) == -1 || pipe(pipe_from_parent_2) == -1 ||
      pipe(pipe_from_child_a) == -1 || pipe(pipe_from_child_b) == -1) {
    fprintf(stderr, "Pipe failed.\n");
    return EXIT_FAILURE;
  }


  FILE *file = fopen("dictionary.txt", "r");
  // FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    perror("Error during 'fopen' function execution");
    return EXIT_FAILURE;
  }

  child_a = fork();

  if (child_a < 0) {
    fprintf(stderr, "Fork failed");
    return EXIT_FAILURE;
  }

  if (child_a == 0) {
    /* child A code */

    int word_counter = 0;
    close(pipe_from_parent_1[1]);

    close(pipe_from_child_a[0]);

    while (1) {
      if (read(pipe_from_parent_1[0], buf, sizeof(buf)) > 0) {
        // printf("Child A read value : %s\n", buf);
      } else {
        break;
      }

      word_counter++;
      write(pipe_from_child_a[1], &word_counter, sizeof(word_counter));
      // printf("Child A write value : %d\n", word_counter);
    }

    close(pipe_from_parent_1[0]);
    close(pipe_from_child_a[1]);

  } else {
    child_b = fork();

    if (child_b < 0) {
      fprintf(stderr, "Fork failed");
      return EXIT_FAILURE;
    }

    if (child_b == 0) {
      /* Child B code */

      int words_with_pipe_counter = 0;
      close(pipe_from_parent_2[1]);

      close(pipe_from_child_b[0]);

      while (1) {
        if (read(pipe_from_parent_2[0], buf, sizeof(buf)) > 0) {
          // printf("Child B read value : %s\n", buf);
        } else {
          break;
        }

        if (strstr(buf, pattern) != NULL) {
          words_with_pipe_counter++;
        }
        write(pipe_from_child_b[1], &words_with_pipe_counter,
              sizeof(words_with_pipe_counter));
        // printf("Child B write value : %d\n", words_with_pipe_counter);
      }

      close(pipe_from_parent_2[0]);
      close(pipe_from_child_b[1]);

    } else {
      /* Parent Code */
      int wn = 0, words = 0;
      int wwp = 0, words_with_pipe = 0;

      close(pipe_from_parent_1[0]);
      close(pipe_from_parent_2[0]);

      close(pipe_from_child_a[1]);
      close(pipe_from_child_b[1]);

      printf("Processing...\n");
      while (fgets(word, sizeof(word), file)) {
        if (strstr(word, pattern) != NULL) {
          wwp++;
        }
        wn++;

        write(pipe_from_parent_1[1], word, (strlen(word) + 1));
        write(pipe_from_parent_2[1], word, (strlen(word) + 1));
        // printf("Parent write value: %s\n", word);

        
        if (read(pipe_from_child_a[0], &words, sizeof(words)) > 0) {
          // printf("Parent read value: %d\n", words);
        } else {
          break;
        }

        if (read(pipe_from_child_b[0], &words_with_pipe,
                 sizeof(words_with_pipe)) > 0) {
          // printf("Parent read value: %d\n", words_with_pipe);
        } else {
          break;
        }
      }

      close(pipe_from_parent_1[1]);
      close(pipe_from_parent_2[1]);

      close(pipe_from_child_a[0]);
      close(pipe_from_child_b[0]);

      
      printf("Number of words: %d\nNumber of words from child process: %d\n",
             wn, words);
      printf(
          "Number of words containing 'pipe': %d\nNumber of words containing "
          "'pipe' from child process: %d\n",
          wwp, words_with_pipe);
    }
  }

  fclose(file);
  return 0;
}
