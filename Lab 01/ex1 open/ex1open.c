#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "t.h"

#define BUFSIZE 1024

int main (int argc, char **argv) {
    int f1, c;
    char b[BUFSIZE], *n1;
    ssize_t f2;
    c = 100;
    n1 = argv[1];

    timestart();
    f1 = open (n1, O_RDONLY);
    if(f1 == -1){
        perror("Nie udalo sie otworzyc pliku");
        exit(EXIT_FAILURE);
    }

    f2 = read (f1, b, c);
    if(f2 == -1){
        perror("Nie udalo sie odczytac pliku");
        exit(EXIT_FAILURE);
    }


	timestop("Uzyskano czas wykonania");

    printf("%s: Przeczytano %d znakow z pliku %s: \"%s\"\n",
           argv[0], c, n1, b);
    close(f1);

    return(0);
}
