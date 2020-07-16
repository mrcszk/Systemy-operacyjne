#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <aio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

// uruchamianie z opcja "-lrt"

/*  -------------------------------------------------------------------------------- */

static void print_type(struct stat *sb);
static void print_ino(const struct stat *sb);
static void print_perms(const struct stat *sb, const char * path);
static void print_linkc(const struct stat *sb);
static void print_owner(const struct stat *sb);
static void print_size(const struct stat *sb);
static void print_laststch(const struct stat *sb);
static void print_lastacc(const struct stat *sb);
static void print_lastmod(const struct stat *sb);
static void print_name(const struct stat *lsb, char *name);
char *getRelatedDateString(long long sec);
int getRelatedDateInt(long long sec);
static void print_content(char *name);
/* -------------------------------------------------------------------------------- */
 
int  main(int argc, char *argv[])
{
  struct stat sb;
 
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
 
  if (stat(argv[1], &sb) == -1) {
    perror("stat");
    exit(EXIT_SUCCESS);
  }
 
  print_type(&sb);
  print_name(&sb, argv[1]);
  print_ino(&sb);
  print_perms(&sb, argv[1]);
  print_linkc(&sb);
  print_owner(&sb);
  print_size(&sb);
  print_laststch(&sb);
  print_lastacc(&sb);
  print_lastmod(&sb);
  print_content(argv[1]);
 
  exit(EXIT_SUCCESS);
}
/* -------------------------------------------------------------------------------- */
 
static void print_type(struct stat *lsb) {
  printf("File type:                ");
  switch (lsb->st_mode & S_IFMT) {
  case S_IFBLK:
    printf("block device\n");
    break;
  case S_IFCHR:
    printf("character device\n");
    break;
  case S_IFSOCK:
    printf("socket\n");
    break;
  case S_IFLNK:
    printf("symbolic link\n");
    break;
  case S_IFREG:
    printf("regular file\n");
    break;
  case S_IFDIR:
    printf("directory\n");
    break;
  case S_IFIFO:
    printf("FIFO queue\n");
    break;
  default:
    printf("unknown?\n");
    break;
  }
}
/* -------------------------------------------------------------------------------- */
 
static void print_ino(const struct stat *sb){
  printf("I-node number:            %ld\n", (long) sb->st_ino);
}
/* -------------------------------------------------------------------------------- */
 
static void print_perms(const struct stat *sb, const char * path) {
    printf("Mode:                     %lo\n",
            (unsigned long) sb->st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
    printf("Your permisions:          read: %s \n                          write: %s \n                          execute: %s\n",
            access(path, R_OK) == 0 ? "yes" : "no",
            access(path, W_OK) == 0 ? "yes" : "no",
            access(path, X_OK) == 0 ? "yes" : "no");
}
/* -------------------------------------------------------------------------------- */
 
static void print_linkc(const struct stat *sb){
  printf("Link count:               %ld\n", (long) sb->st_nlink);
}
/* -------------------------------------------------------------------------------- */
 
static void print_owner(const struct stat *sb) {
  struct passwd *pw = getpwuid(sb->st_uid);
  struct group *gr = getgrgid(sb->st_gid);

  printf("Ownership:                %s(%ld)   %s(%ld)\n", pw->pw_name,
         (long)sb->st_uid, gr->gr_name, (long)sb->st_gid);
}
/* -------------------------------------------------------------------------------- */
 
static void print_size(const struct stat *sb){

    long long size = sb -> st_size;
    char* postfix = "bytes";

    if (size > 1048576){
        size = size / 1048576;
        postfix = "MB";
    }
    else if (size > 1024){
        size = size / 1024;
        postfix = "kB";
    }

    printf("Preferred I/O block size: %ld bytes\n", (long) sb->st_blksize);
    printf("File size:                %lld %s\n", size, postfix);
    printf("Blocks allocated:         %lld\n",(long long) sb->st_blocks);
}
/* -------------------------------------------------------------------------------- */
 
static void print_laststch(const struct stat *sb) {
  time_t now;
  time(&now);
  long long seconds;
  seconds = difftime(now, sb->st_ctime);
  char *related_date_string = getRelatedDateString(seconds);
  int related_date_int = getRelatedDateInt(seconds);
  printf("Last status change:       %s", ctime(&sb->st_ctime));
  printf("Last status change:       %d %s\n", related_date_int,
         related_date_string);
}
/* -------------------------------------------------------------------------------- */
 
static void print_lastacc(const struct stat *sb) {
  time_t now;
  time(&now);
  long long seconds;
  seconds = difftime(now, sb->st_atime);
  char *related_date_string = getRelatedDateString(seconds);
  int related_date_int = getRelatedDateInt(seconds);
  printf("Last file access:         %s", ctime(&sb->st_atime));
  printf("Last file access:         %d %s\n", related_date_int,
         related_date_string);
}
/* -------------------------------------------------------------------------------- */
 
static void print_lastmod(const struct stat *sb) {
  time_t now;
  time(&now);
  long long seconds;
  seconds = difftime(now, sb->st_mtime);
  char *related_date_string = getRelatedDateString(seconds);
  int related_date_int = getRelatedDateInt(seconds);
  printf("Last file modification:   %s", ctime(&sb->st_mtime));
  printf("Last file modification:   %d %s\n", related_date_int,
         related_date_string);
}
/* -------------------------------------------------------------------------------- */

char *getRelatedDateString(long long sec) {
  char *result;
  if (sec < 60) {
    result = "second(s) ago";
  } else if (sec >= 60 && sec < 3600) {
    result = "minute(s) ago";
  } else if (sec >= 3600 && sec < 86400) {
    result = "hour(s) ago";
  } else {
    result = "day(s) ago";
  }
  return result;
}

/* --------------------------------------------------------------------------------
 */

int getRelatedDateInt(long long sec) {
  int result = 0;
  if (sec >= 60 && sec < 3600) {
    result = sec / 60;
  } else if (sec >= 3600 && sec < 86400) {
    result = sec / 3600;
  } else {
    result = sec;
  }
  return result;
}

/* --------------------------------------------------------------------------------
 */
 
static void print_name(const struct stat *lsb, char *name) {
  char *bname = basename(name);
  char *lname = (char *)malloc(128 * sizeof(char));

  if (S_ISLNK(lsb->st_mode)) {
    int count = readlink(name, lname, sizeof(lname));
    if (count >= 0) {
      lname[count] = '\0';
      printf("Name of the file:         %s -> %s\n", name, lname);
    } else {
      printf("Name of the file:         Can not read link from file %s\n",
             bname);
    }
  } else
    printf("Name of the file:         %s\n", bname);
}
/* -------------------------------------------------------------------------------- */

static void print_content(char *name){
    printf("\n\nWould you like to read %s content?[y/n]", name);
    char if_read;
    scanf("%c", &if_read);

    if(if_read == 'n'){
        return;
    }

    int fd;
    ssize_t bytes;
    char* buff[512];

    fd = open(name, O_RDONLY);
    if (fd == -1){
        perror("Unable to open this file!");
        exit(EXIT_FAILURE);
    }

    struct aiocb* cb;
    cb = malloc(sizeof(struct aiocb));

    cb -> aio_fildes = fd;
    cb -> aio_offset = 0;
    cb -> aio_buf = buff;
    cb -> aio_nbytes = sizeof(buff);

    if (aio_read(cb) == -1){
        close(fd);
        printf("Operation failed\n");
        exit(EXIT_FAILURE);
    }

    while(aio_error(cb) == EINPROGRESS){
        printf("%p", cb ->aio_buf);
    }


    close(fd);
}
