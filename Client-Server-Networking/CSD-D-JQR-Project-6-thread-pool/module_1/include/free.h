#ifndef _FREE_H
#define _FREE_H

#define FREE(a) \
    if (a)      \
        free(a);\
    (a) = NULL;

    
#endif