#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAX 1024

int main(int argc, char* argv[]) {

    int fd1, fd2, nwritten, nread, nchars;
    char *file_name = argv[1];

    char buff_to_read[MAX];

    nchars = 10;

    fd1  = open(file_name, O_RDWR);
    fd2 = open(file_name, O_RDWR);

    char* buff = "01234567891";    // specjalnie 11 znaków, aby sprawdzic 
                                   // czy tylko 10 zostanie odczytanych
    nwritten = write(fd1, buff, nchars);
    nread = read(fd2, buff_to_read, nchars);

    printf("%s", buff_to_read);


    close(fd1);
    close(fd2);

    return 0;
}
