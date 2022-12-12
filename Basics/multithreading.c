#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


void *readfile(void *);

int main(int argc, char **argv) {
    int numfiles = argc -1; //will be the number of files that the user gives
    int i;
    void *result = NULL;
    int ret;
    pthread_t *threads = NULL;

    if (argc == 1){
        perror("usage: ./readfiles [file1] [file2]...\n"); //automatically sends to stderr
        return 1;
    }
    threads = malloc(sizeof(*threads) * numfiles);
    if (threads == NULL){
        fprintf(stderr, "Unable to allocate for threads. Exiting...\n");
        return 1;
    }

    for (i = 0; i < numfiles; i++){
        ret = pthread_create(&threads[i], NULL, &readfile, argv[i+1]);
        if (ret != 0){
            perror("Unable to create thread. Exiting...\n");
        }
    }
    
    for (i = 0; i < numfiles; i++){
        result = NULL;
        pthread_join(threads[i], &result);
        if (result != NULL){ // on join success
            printf("%s\n", (char *)result);
            free(result);
            result = NULL;
        }
    }

    free(threads);
    return 0;
}

void *readfile(void *arg){

    char *filename = (char *)arg;
    char *buffer = NULL;

    size_t buffersize = 200;
    size_t bytesread = 0;
    
    FILE *fp = NULL;
    pthread_t thread = pthread_self();

    fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf("%ld unable to open: %s\n", thread, filename);
        pthread_exit(NULL);
    }
    buffer = calloc(buffersize, sizeof(char));

    if (buffer == NULL){
        perror("Unable to allocate buffer. Exiting...\n");
        pthread_exit(NULL);
    }

    bytesread = fread(buffer, sizeof(char), buffersize-1, fp);
    fclose(fp);
    printf("%ld read %ld bytes: %s\n", thread, bytesread, filename);

    pthread_exit(buffer);

}
