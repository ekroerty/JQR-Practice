/**
 * @file htable.h
 * @author Emma Roerty
 * @brief Header file for hash table data structure
 * @version 1.1
 * @date 2022-12-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _HTABLE_H
#define _HTABLE_H

#include <stdint.h>
#include <stdbool.h>
#include "./list.h"


/**
 * @brief user defined free function
 * 
*/

typedef void
(* FREE_F) (void *);

/**
 * @brief hash function that will be an attribute of the hash table
*/

typedef uint32_t
(* HASH_F) (void *, const uint32_t);

/**
 * @brief user defined compare function
 * 
 * @param item1 to compare
 * @param item2 to compare
 * 
 * @return if item1 == item2 return 0
 *          if item1 > item2 return 1
 *          if item1 < item2 return -1
 * 
*/

typedef int
(* CMP_F) (const void *, const void *);

/**
 * @brief hash table node instance
 * 
 * @param p_key the key that the user gives prior to being hashed
 * @param p_value the data being held by that key
 * @param p_next pointer to the next node with the same hash value
 * @param free_f user defined free function
*/

typedef struct htable_node_t
{
    void *                 p_key;
    void *                 p_value;
    FREE_F                 free_f;
} htable_node_t;

/**
 * @brief hash table instance
 * 
 * @param size user-defined size for the hash_val array
 * @param hash_f user-defined hash function
 * @param comp_f user-defined compare function to check for equivalency
 * @param p_hash_vals of pointers to node types
*/

typedef struct htable_t
{
    HASH_F           hash_f;
    CMP_F            comp_f;
    list_t       **  p_hash_vals;
    uint32_t         load_fact_num;
    uint32_t         load_fact_denom;
    uint32_t         max_size;
    uint32_t         curr_size;
    uint32_t         capacity;
} htable_t;

/**
 * @brief can create a hash table instance
 * 
 * @param hash_f user defined hash function
 * @param comp_f user-defined compare function
 * @param size of the number of "boxes" the hash table has
 * 
 * @return hash table instance or null on error
*/

htable_t *
htable_create (HASH_F, CMP_F);

/**
 * @brief can find an item in a hash table
 * 
 * @param p_htable table to search through
 * @param p_key key to seach for
 * 
 * @return node associated with p_key, or null if failure
*/

htable_node_t *
htable_find (htable_t *, void *);


/**
 * @brief can insert a value with a key to the hash table
 * 
 * @param p_htable table to insert into
 * @param p_key to insert with
 * @param p_value data being inserted
 * @param free_f free function if the data is a complex data type
 * 
 * @return true if successful, false if not 
*/

bool
htable_insert (htable_t **, void *, void *, FREE_F);


/**
 * @brief can remove selected items from a hash table
 * 
 * @param p_htable instance to remove from
 * @param p_key key of value to remove
 * 
 * @return node that was removed or null on error
*/

htable_node_t *
htable_remove (htable_t *, void *);

/**
 * @brief updates a node within the table, should be checked if user has permissions
 * 
 * @param p_node pointer to the node to update
 * @param p_value value to update to
 * @param free_f free function for new value
 */

bool
htable_update (htable_node_t * p_node,
                             void * p_value, FREE_F free_f);
/**
 * @brief can destroy all items in a hash table
 * 
 * @param p_htable htable pointer to destroy all items of
 * 
 * @return true if successful, false if not 
*/


bool
htable_destroy_items (htable_t *);

/**
 * @brief can destroy a hash table context
 * 
 * @param pp_htable pointer to htable pointer to destroy
 * 
 * @return true if successful, false if not 
*/

bool
htable_destroy (htable_t **);

#endif

/* END OF FILE */

