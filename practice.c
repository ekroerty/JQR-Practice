#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printarray(int array[], int elements);
void sum(int array[], int elements);
void string_stuff(char *string);


int main(){
    int x = 10;
    int *y = &x; //y is a pointer, and it points to the address of x
    int *z = y;

    char a;

    printf("POINTER PRACTICE\n\n");
    printf("The value of x should be 10. The value of x is %d\n", x);
    printf("The value of y should be %p. The value of y is %p\n", &x, y);
    printf("The value of z should be %p. The value of z is %p\n\n", &x, z);

    printf("The value of x from z should be 10. The value of x from z is %d\n\n", *z);

    printf("The address of y is %p. The address of z is %p.\n", &y, &z);


    printf("\n------------------------------\n\n");
    printf("MEMORY PRACTICE\n\n");
    printf("a will be a random character because I did not initialize it and it's address is a random value on the stack. a =  %c\n", a);
    
    printf("\n\n------------------------------\n\n");
    printf("PASS BY REFERENCE PRACTICE\n\n");

    int nums[] = {1,2,3,4,5,6,7,8,9};
    int primes[] = {2,3,5,7};

    int nums_elems = sizeof(nums)/4;
    int primes_elems = sizeof(primes)/4;

    printarray(nums, nums_elems);
    sum(nums, nums_elems);
    printarray(primes,primes_elems);
    sum(primes, primes_elems);

    printf("\n\n------------------------------\n\n");
    printf("STRINGS PRACTICE\n\n");

    char *string1 = "Hello!";
    char string2[] = "Goodbye!";
    char int_string[] = "1234 splendas in ur coffee Stanley";
    char *ptr;
    long i;

    char *temp;
    char buffer[50] = { 0 };

    temp = fgets(buffer, sizeof(buffer), stdin);

    printf("%s\n", temp);

    i = strtol(int_string, &ptr, 10);
    printf("Orginial string: %s\n", int_string);
    printf("Integer from the string: %ld\n", i);
    printf("Remainder of the string that is not an int: %s\n\n", ptr);

    printf("%s\n", string1);
    printf("%s\n", string2);

    printf("Length of the string1: %ld\n", strlen(string1));
    printf("Length of the string2: %ld\n", strlen(string2));
    printf("Null terminator at the end of the string: %d\n", string2[8]); //notw working for whatever reason

    

    return 0;


}

void printarray(int array[], int elements){
    printf("Array Address: %p, Element Count: %d\n", array, elements);

    for (int i = 0; i < elements; i++){
        printf("Index #%d: %d", i, array[i]);
        printf("\n");
    }
}

void sum(int array[], int elements){
    int sum = 0;

    /*
        This section of code makes sure that the pointers is not a null pointer. 
        If a null pointer is passed in, there will be a seg fault
    */
    if (NULL == array){ 
        fprintf(stderr,"Array is NULL\n");
        printf("%d",sum);
    }

    for (int i = 0; i < elements; i++){
        sum += array[i];
    } 
    printf("Sum of all numbers in the array: %d\n", sum);

}

void string_stuff(char string[]){
    printf("Placeholder");
}