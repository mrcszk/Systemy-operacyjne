#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE* fp;      
  char c;

  if (argc == 1) {
    fp = stdin;
  } else {
    fp = fopen(argv[1], "r");
  }
  
  if (fp == NULL) {
        perror("ERROR: Failed opening file!");
        exit(EXIT_FAILURE);
  }

  while ((c = getc(fp)) != EOF) {
    printf("%c", c);
  }

  if (fclose(fp) == -1){
        perror("ERROR: Failed closing files!");
        exit(EXIT_FAILURE);
  }
  return 0;
}
