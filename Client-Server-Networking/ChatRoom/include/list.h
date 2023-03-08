/**
 * @file list.h
 * @author Emma Roerty
 * @brief Header file for list data structure
 * @version 1.1
 * @date 2022-12-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _LIST_H
#define _LIST_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief user defined free function provided if given data
 *        is a complex data structure
*/

typedef void 
(* FREE_F) (void *);

/**
 * @brief user defined compare function
 * 
 * @param item1 to compare
 * @param item2 to compare
 * 
 * @return if item1 == item2 return 0
 *         if item1 > item2 return 1
 *         if item1 < item2 return -1
 * 
*/

typedef int
(* CMP_F) (const void *, const void *);

/**
 * @brief represents a node in the list
 * 
 * @param p_data the value being held in the element
 * @param p_next points to the next element in the list
 * @param free_f free function given by the user if the data is a
 *               complex data structure
*/

typedef struct list_node_t
{
    void *                 p_data;
    struct list_node_t *   p_next;
    FREE_F                 free_f;
} list_node_t;

/**
 * @brief list data structure
 * 
 * @param size size of the list
 * @param p_head pointer to the head of the list in memory
 * @param p_tail pointer to the tail of the list in memory. tail->next should equal head
 * @param comp_f user-defined compare function
*/

typedef struct list_t
{
    uint32_t            size; 
    list_node_t *       p_head;
    list_node_t *       p_tail;
    CMP_F               comp_f;
} list_t;

/**
 * @brief creates an list instance
 * 
 * @param comp_f user-defined compare function
 * 
 * @returns a pointer to the list instance, or null on error
*/

list_t *
list_create (CMP_F);

/**
 * @brief gives the size of the list
 * 
 * @param p_list the list to return the size of
 * 
 * @returns size of list, or -1 on error
*/

int32_t 
list_size (list_t *);

/**
 * @brief can add an element to the end of the list
 * 
 * @param p_list list to append data onto
 * @param p_data value to append to the end of the list
 * 
 * @returns true if successful, false if not
*/

bool
list_append (list_t *, void *, FREE_F);

/**
 * @brief can add an element to the beginning of the list
 * 
 * @param list list to prepend data onto
 * @param data value to prepend onto the list
 * 
 * @return true if successful, false if not
*/

bool
list_prepend (list_t *, void *, FREE_F);

/**
 * @brief can insert an element anywhere within the list
 * 
 * @param list to insert into
 * @param data to insert into the list
 * @param n at what index to insert the given data
 * 
 * @return true if successful, false if not
*/

bool 
list_insert (list_t *, void *, uint32_t , FREE_F);

/**
 * @brief removes the last element in the list
 * 
 * @param list to remove tail from
 * 
 * @return the tail value
*/

void *
list_pop_tail (list_t *);

/**
 * @brief removes the first element from the list
 * 
 * @param list list to remove the first element from
 * 
 * @return the head value
*/

void *
list_pop_head (list_t *);

/**
 * @brief removes any element from the list at any position
 * 
 * @param list to remove the element from
 * @param n index at which the element is to be removed
 * 
 * @return the element at given index
*/

void *
list_remove (list_t *, unsigned int);

/**
 * @brief can get an element from n position in the list
 * 
 * @param list to get element from
 * @param n position to get element from
 * 
 * @return data at n element
*/

void *
list_index (list_t *, unsigned int);

/**
 * @brief can find the first occurrance of an item
 * 
 * @param list to iterate over
 * @param item to find
 * 
 * @return the index of the first occurrance of given item, 
 *         -1 on error
*/

int32_t
list_first_occurrence (list_t *, void *);

/**
 * @brief sorts the list based on the user-defined compare function
 * 
 * @param list list to be sorted
 * 
 * @return sorted list
*/

bool
list_merge_sort (list_t *);

/**
 * @brief destroys all elements within an list using
 *        the correct deallocation function
 * 
 * @param list to destroy elements from
 * 
 * @returns true if successful, false if not
*/

bool
list_destroy_items (list_t *);

/**
 * @brief destroys the list index completely
 * 
 * @param list instance to destroy
 * 
 * @return true if successful, false if not
*/

bool
list_destroy (list_t **);

void * 
list_remove_val(list_t *, void *);

#endif

/* END OF FILE */
