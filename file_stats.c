#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MYFILE "testfile"

int main (){
    struct stat sb = { 0 };
    int result;

    result = stat(MYFILE, &sb);

    if (result != 0){
        fprintf(stderr, "Unable to get stats. Exiting gracefully...\n");
        return 1;
    }
    printf("UID: %d\n", sb.st_uid);
    printf("Mode: %d\n", sb.st_mode);
}