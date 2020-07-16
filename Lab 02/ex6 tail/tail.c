#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void tail(char *path)
{
    char c;
    int fd, ncount;
    off_t tail, end;

    fd = open(path, O_RDONLY);
    tail = end = lseek(fd, 1, SEEK_END);
    ncount = 0;

    while (ncount < 11 && tail > 0) {
      if (pread(fd, &c, 1, tail - 1) == -1) {
          perror("Fail");
          exit(EXIT_FAILURE);
      }

      if (c == '\n') {
          ncount++;

          if (ncount == 11) {
              tail++;
          }
      }

      tail--;
    }

    while (tail < end) {
      if (pread(fd, &c, 1, tail) == -1) {
          perror("Fail");
          exit(EXIT_FAILURE);
      }

      putchar(c);
      tail++;
    }

    close(fd);
    return;
}

int main(int argc, char* argv[]) {
    tail(argv[1]);
    
    return 0;
}
