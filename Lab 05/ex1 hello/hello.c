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

    for(int i=0; i<10; i++){
        rc = pthread_create(&threads[i], NULL, PrintHello, (void *)&i);
        if (rc) {
            printf("Return code: %d\n", rc);
            exit(-1);
        }
        
    }
    
    sleep(1);
    return 0;
}
// ----------------------------------------------------------
 
void *PrintHello(void *arg){
    
    printf("Next boring 'Hello World!' version! >> %d\n", *((int*) arg));
    return NULL;
}
