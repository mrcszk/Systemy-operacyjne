#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 5

int main(int argc, char *argv[]) {
  struct rlimit limit;
  char buf[BUFSIZE] = {'a', 'a', 'a', 'a', 'a'};
  char *filename;
  int bytes = 0, towrite, smin = 0, smax = 10000, sdef = 100, wrote, fd,
      s = sdef;

  if (argc < 2 || argc > 3) {
    fprintf(stderr, "usage: %s [number_of_bites] filename\n", argv[0]);
    return 1;
  } else {
    if (argc == 3) {
      if (!atoi(argv[1])) {
        fprintf(stderr, "number_of_bites must be integer value.\n");
        return 1;
      } else {
        filename = argv[2];
        s = atoi(argv[1]);
      }
    } else {
      filename = argv[1];
    }
  }

  if ((s < smin) || (s > smax))
    s = sdef;

  /* reading file size limit */
  getrlimit(RLIMIT_FSIZE, &limit);
  printf("RLIMIT_FSIZE: cur=%ld, max=%ld\n", limit.rlim_cur, limit.rlim_max);
  fflush(stdout);

  printf("Writing %d bytes into %s file...\n", s, filename);
  fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  do {
    towrite = s - bytes;
    towrite = towrite < BUFSIZE ? towrite : BUFSIZE;
    wrote = write(fd, buf, towrite);
    bytes += wrote;
    if (wrote == -1)
      break;
  } while ((wrote > 0) && (bytes < s));
  close(fd);

  return 0;
}
