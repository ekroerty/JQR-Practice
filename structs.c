#include <stdio.h>
#include "structs.h"

static void print_position(struct coordinate *c);

int main(){

    coord coord1 = {500, 125};

    printf("X: %d\n", coord1.x);
    printf("Y: %d\n", coord1.y);


    /* Pointer to a Struct */
    struct coordinate *pcoord1 = &coord1;

    pcoord1->x += 10;
    pcoord1->y += 10;

    // printf("X: %d\n", pcoord1->x); // dereferences pointer to access x
    // printf("Y: %d\n", pcoord1->y);    

    print_position(pcoord1);

    return 0;
}

static void print_position(coord *c){
    printf("X: %d Y: %d\n", c->x, c->y);
}