#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void backward(char* path) {
  char s[256], c;
  int i, fd;
  off_t where;

  if ((fd = open(path, O_RDONLY)) == -1) {
    fprintf(stderr, "./backward: open: %s: %s\n", path, strerror(errno));
    exit(EXIT_FAILURE);
  }

  where = lseek(fd, 1, SEEK_END);
  i = sizeof(s) - 1;
  s[i] = '\0';
  do {
    where = lseek(fd, -2, SEEK_CUR);
    switch (read(fd, &c, 1)) {
      case 1:
        if (c == '\n') {
          printf("%s", &s[i]);
          i = sizeof(s) - 1;
        }
        if (i <= 0) {
          errno = E2BIG;
          fprintf(stderr,
              "./backward: read: %s: Buffer is too small to accommodate the "
              "entire line\n",
              path);
          exit(EXIT_FAILURE);
        }
        s[--i] = c;
        break;
      case -1:
        fprintf(stderr, "./backward: read: %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
        break;
      default: /* impossible */
        errno = 0;
        fprintf(stderr, "./backward: read: %s: Unhandled error\n", path);
        exit(EXIT_FAILURE);
    }
  } while (where > 0);

  printf("%s", &s[i]);
  close(fd);
  return;
}


int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s path\n", argv[0]);
    return -1;
  } else
    backward(argv[1]);

  return 0;
}


