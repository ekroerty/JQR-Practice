#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    /*
        malloc example
    */
    int n;
    int *ptr;
    int *temp;
    if (argc == 2){
        n = atoi(argv[1]);
        ptr = malloc(n * sizeof(*ptr)); 
        /*
            doing *ptr ensures it's the right size for whatever data type ptr is
        */
        if (ptr == NULL){
            fprintf(stderr, "Memory allocation failed! Exiting gracefully...\n");
            return -1;
        }
        printf("Size: %ld\n", n * sizeof(*ptr));
        /*
            realloc example
        */
        n *= 2;
        temp = realloc(ptr, n * sizeof(*ptr));
        if (temp == NULL){
            fprintf(stderr, "Failed to reallocate memory! Exiting gracefully...\n");
            free(ptr);
            return -1;
        }
        ptr = temp; /* If memory didn't have to move, this commmand will do nothing*/

        free(ptr);
        ptr = NULL;
    }
    /*
        calloc example is the exact same, but substitute the * for a comma in the function call
    */

    return 0;
}