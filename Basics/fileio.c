#include <stdio.h>
#include <string.h>

#define MYFILE "testfile"

int main() {
    FILE *fp;
    size_t bytes;
    char *outbuffer = "I want to write to a file!\n";
    char inbuffer[10] = { 0 };
    int status;

    /* Open a file */
    fp = fopen(MYFILE, "w");
    if (fp == NULL){
        fprintf(stderr, "Unable to open %s to write. Exiting gracefully...\n", MYFILE);
        return 1;
    }

    /* Write to a file */
    bytes = fwrite(outbuffer, sizeof(char), strlen(outbuffer), fp);
    printf("%ld bytes written\n", bytes);

    /* Close a file */
    status = fclose(fp);
    if (status != 0){
        fprintf(stderr, "Unable to close file. Exiting gracefully...\n");
        return 1;
    }

    /* Open a file */
    fp = fopen(MYFILE, "r");
    if (fp == NULL){
        fprintf(stderr, "Unable to open %s to write. Exiting gracefully...\n", MYFILE);
        return 1;
    }

    /* Read from file, allows you to only take chunks of the file at a time, you 
       don't have to have huge buffers
    */
    while((bytes = fread(inbuffer, sizeof(char), sizeof(inbuffer) - 1, fp)) > 0){
        inbuffer[bytes] = 0;
        printf("Bytes: %ld %s\n", bytes, inbuffer);

    }

    /* Close a file */
    status = fclose(fp);
    if (status != 0){
        fprintf(stderr, "Unable to close file. Exiting gracefully...\n");
        return 1;
    }

    return 0;
}