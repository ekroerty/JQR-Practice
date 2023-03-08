/**
 * @file free.h
 * @author Emma Roerty
 * @brief Header file containing FREE macro to be 
 *        used by all data structures
 * @version 1.0
 * @date 2022-12-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _FREE_H
#define _FREE_H

#define FREE(a) \
    if (a)      \
        free(a);\
    (a) = NULL;

    
#endif

/* END OF FILE */
