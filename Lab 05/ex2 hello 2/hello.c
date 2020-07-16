#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// gcc -pthread -o hello hello.c

// ----------------------------------------------------------
 
void *PrintHello(void *arg);
// ----------------------------------------------------------
 
int main(int argc, char *argv[]){
    pthread_t threads[10];
    int rc = 0;
    intptr_t i = 0;

    for(i=0; i<10; i++){        
        printf("In main: creating thread %d\n", (int)i);
        rc = pthread_create(&threads[i], NULL, PrintHello, (void *)&i);
        if (rc) {
            fprintf(stderr, "Failed to create thread #%d - %s\n", (int)i, strerror(rc));
            exit(EXIT_FAILURE);
        }  
    } 

    for (i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
        if (rc) {
            fprintf(stderr, "Failed to create thread #%d - %s\n", (int)i, strerror(rc));
            exit(EXIT_FAILURE);
    }
    }

    printf("End of the main thread!\n");
    return 0;
}
// ----------------------------------------------------------
 
void *PrintHello(void *arg){
    //sleep(1);
    printf("Next boring 'Hello World!' version! >> %d\n", *((int*) arg));
    return NULL;
}
