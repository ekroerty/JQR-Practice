/**
 * @file htable.c
 * @author Emma Roerty
 * @brief Source file for hash table data structure
 * @version 1.1
 * @date 2022-12-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../include/htable.h"
#include "../include/free.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HT_FAILURE NULL
#define SUCCESS true
#define N_FAILURE false

#define START_CAPACITY 2
#define LOAD_FACT_NUM 3
#define LOAD_FACT_DENOM 4

void print_htable (htable_t *);

void
print_list(list_t *);

htable_t *
htable_create (HASH_F hash_f, CMP_F comp_f)
{

    if ((!hash_f) || (!comp_f))
    {
        return HT_FAILURE;
    }

    htable_t * p_htable = calloc(1, sizeof (htable_t));

    if (!p_htable)
    {
        return HT_FAILURE;
    }

    p_htable->hash_f          = hash_f;
    p_htable->comp_f          = comp_f;
    p_htable->curr_size       = 0;
    p_htable->capacity        = START_CAPACITY;
    p_htable->load_fact_num   = LOAD_FACT_NUM;
    p_htable->load_fact_denom = LOAD_FACT_DENOM;
    // ensures that the max_size is a whole number using floor division
    p_htable->max_size        =
            ((p_htable->capacity * p_htable->load_fact_num)
                                        / p_htable->load_fact_denom);
    p_htable->p_hash_vals = calloc(p_htable->capacity, sizeof(list_t *));

    if (!p_htable->p_hash_vals)
    {
        FREE(p_htable);
        return HT_FAILURE;
    }

    return p_htable;
}

void node_free (void * p_node)
{
    htable_node_t * p_htable_node = (htable_node_t *)p_node;

    if (p_htable_node->free_f)
    {
        p_htable_node->free_f(p_htable_node->p_value);
    }

    FREE(p_htable_node);
}

void htable_node_insert(htable_t * p_htable,
                        htable_node_t * p_node)
{
    uint32_t capacity = p_htable->capacity;
    uint32_t key_idx = p_htable->hash_f(p_node->p_key, capacity)%capacity;

    list_t * p_list = ((list_t *)p_htable->p_hash_vals[key_idx]);

    if (!p_list)
    {
        p_list = list_create(p_htable->comp_f);
    }

    list_prepend(p_list, p_node, node_free);
    p_htable->p_hash_vals[key_idx] = p_list;
    p_htable->curr_size++;

    printf("Inserting %s into new hash table\n", (char *)p_node->p_key);

    return;
}

htable_t *
htable_rehash(htable_t ** pp_htable)
{
    if ((!pp_htable) || (!(pp_htable)) || 
            ((*pp_htable)->max_size >= ((*pp_htable)->curr_size + 1)))
    {
        htable_destroy(pp_htable);
        return HT_FAILURE;
    }

    HASH_F hash_func_cpy = (*pp_htable)->hash_f;
    CMP_F  comp_func_cpy = (*pp_htable)->comp_f;

    htable_t * p_new_htable = htable_create(hash_func_cpy, comp_func_cpy);

    if (!p_new_htable)
    {
        htable_destroy(pp_htable);
        return HT_FAILURE;
    }

    printf("Creating new table!\n");

    FREE(p_new_htable->p_hash_vals);
    uint32_t new_cap = ((*pp_htable)->capacity * 2);

    p_new_htable->capacity    = new_cap;
    p_new_htable->p_hash_vals = calloc(p_new_htable->capacity,
                                         sizeof(list_t *));

    if (!p_new_htable->p_hash_vals)
    {
        htable_destroy(&p_new_htable);
        htable_destroy(pp_htable);
        return HT_FAILURE;
    }

    p_new_htable->max_size    =
            ((p_new_htable->capacity * p_new_htable->load_fact_num)
                                        / p_new_htable->load_fact_denom);

    list_t * p_list = NULL;

    
    for (uint32_t idx = 0; idx < (*pp_htable)->capacity; idx++)
    {
        if (!(*pp_htable)->p_hash_vals[idx])
        {
            continue;
        }

        if (0 == (*pp_htable)->p_hash_vals[idx]->size)
        {
            list_destroy(&p_list);
            continue;
        }

        p_list = (list_t *)(*pp_htable)->p_hash_vals[idx];

        htable_node_t * p_node = NULL;

        while (p_list->size)
        {
            p_node = list_pop_head(p_list); 
            printf("Removing %s from old table\n", (char *)p_node->p_key);

            htable_node_insert(p_new_htable, 
                                p_node);
        }

        list_destroy(&p_list);
    }

    FREE((*pp_htable)->p_hash_vals);
    FREE(*pp_htable);
    return p_new_htable;

}

bool
htable_update (htable_node_t * p_node,
                             void * p_value, FREE_F free_f)
{
    if (!p_node)
    {
        return N_FAILURE;
    }

    if (p_node->free_f)
    {
        p_node->free_f(p_node->p_value);
    }

    p_node->p_value = p_value;
    p_node->free_f = free_f;

    return SUCCESS;
}

bool
htable_insert
        (htable_t ** pp_htable, void * p_key, void * p_value, FREE_F free_f)
{

    if ((!pp_htable) || (!p_key) || (!p_value) || (!*pp_htable))
    {
        htable_destroy(pp_htable);
        return N_FAILURE;
    }

    htable_node_t * p_temp = htable_find(*pp_htable, p_key);

    if (p_temp)
    {
        printf("Username already found.\n");
        // update_node(p_temp, p_value, free_f);
        p_temp = NULL;
        return N_FAILURE;
    }

    if ((*pp_htable)->max_size < ((*pp_htable)->curr_size + 1))
    {
        printf("About to rehash...\n");
        (*pp_htable) = htable_rehash(pp_htable);
    }

    if (!*pp_htable)
    {
        return N_FAILURE;
    }

    htable_node_t * p_node = calloc(1, sizeof(htable_node_t));

    if (!p_node)
    {
        htable_destroy(pp_htable);
        return N_FAILURE;
    }

    p_node->p_key     = p_key;
    p_node->p_value   = p_value;
    p_node->free_f    = free_f;
    uint32_t capacity = (*pp_htable)->capacity;
    uint32_t key_idx  = (*pp_htable)->hash_f(p_key, capacity)%capacity;
    //index must be between 0 and table capacity

    list_t * p_list = (*pp_htable)->p_hash_vals[key_idx];

    if (!p_list)
    {
        p_list = list_create((*pp_htable)->comp_f);
    }

    list_prepend(p_list, p_node, node_free);
    ((*pp_htable)->p_hash_vals[key_idx]) = p_list;
    (*pp_htable)->curr_size++;

    return SUCCESS;
}

htable_node_t *
htable_find (htable_t * p_htable, void * p_key)
{

    if ((!p_htable) || (!p_key))
    {
        return HT_FAILURE;
    }

    //index must be between 0 and table size
    uint32_t        capacity    = p_htable->capacity;
    uint32_t        key_idx     = p_htable->hash_f(p_key, capacity)%capacity;
    list_t * p_list = p_htable->p_hash_vals[key_idx];

    if (!p_list)
    {
        return HT_FAILURE;
    }

   int32_t idx = list_first_occurrence(p_list, p_key);
   htable_node_t * p_node = list_index(p_list, idx);

   return p_node;

}

htable_node_t *
htable_remove (htable_t * p_htable, void * p_key)
{

    if ((!p_htable) || (!p_key))
    {
        return HT_FAILURE;
    }

    // probably not the issue
    htable_node_t * p_node = htable_find(p_htable, p_key);

    if (!p_node)
    {
        return HT_FAILURE;
    }

    //index must be between 0 and table size
    uint32_t        capacity    = p_htable->capacity;
    uint32_t        key_idx     = p_htable->hash_f(p_key, capacity)%capacity;
    list_t * p_list = p_htable->p_hash_vals[key_idx];

    int32_t idx = list_first_occurrence(p_list, p_key);
    htable_node_t * p_rem_node = list_remove(p_list, idx);

    if (!p_rem_node)
    {
        return HT_FAILURE;
    }

    p_htable->curr_size--;
    return p_rem_node;
}

bool
htable_destroy_items (htable_t * p_htable)
{

    if (!p_htable)
    {
        return N_FAILURE;
    }

    if (!p_htable->p_hash_vals)
    {
        return SUCCESS;
    }

    uint32_t        capacity = p_htable->capacity;

    for (uint32_t iter = 0; iter < capacity; iter++)
    {
        list_destroy(&p_htable->p_hash_vals[iter]);
    }

    return SUCCESS;
}

bool
htable_destroy (htable_t ** pp_htable)
{

    if ((!pp_htable )|| (!*pp_htable))
    {
        return N_FAILURE;
    }

    bool des = htable_destroy_items(*pp_htable);

    if (!des)
    {
        return N_FAILURE;
    }

    FREE((*pp_htable)->p_hash_vals);
    FREE(*pp_htable);
    pp_htable = NULL;
    return SUCCESS;
}

#if 0

void print_list (list_t * p_cll)
{
    list_node_t *node = p_cll->p_head;
    // printf("list: ");
    for (uint32_t i = 0; i < p_cll->size; i++)
    {
        printf("%s: ", (char *)((htable_node_t *)node->p_data)->p_key);
        printf("%s, ", (char *)((htable_node_t *)node->p_data)->p_value);
        node = node->p_next;
    }
    printf("\n");
}

void print_htable (htable_t * p_htable)
{
    list_t * p_list = NULL;

    if (!p_htable)
    {
        printf("Hash table is null.\n");
        return;
    }

    printf("\nHash Table: \n");
    for (uint32_t iter = 0; iter < p_htable->capacity; iter ++)
    {
        printf("%d: ", iter);
        p_list = (list_t *)p_htable->p_hash_vals[iter];

        if (p_list)
        {
            print_list(p_list);
        }
        else
        {
            printf ("Empty.\n");
        }

    }
}
#endif

#if 0
void print_htable_strs (htable_t * p_htable)
{
    printf("\nHash Table: \n");

    for (uint32_t iter = 0; iter < p_htable->capacity; iter ++)
    {
        printf("%d: ", iter);

        if (NULL != p_htable->p_hash_vals[iter].p_value)
        {
            printf ("%s: ", (char *)((htable_node_t *)(p_htable->p_hash_vals[iter].p_value))->p_key);
            printf ("%ld, ", (*(uint64_t *)((htable_node_t *)p_htable->p_hash_vals[iter].p_value)->p_value));
            htable_node_t * p_curr_node = p_htable->p_hash_vals[iter].p_value;

            while (NULL != p_curr_node->p_next)
            {
                p_curr_node = p_curr_node->p_next;
                printf("%s: ", (char *)p_curr_node->p_key);
                printf("%ld, ", (*(uint64_t *)p_curr_node->p_value));

            }

            printf("\n");
        }

        else
        {
            printf ("Empty.\n");
        }
    }
}

void print_htable_pass(htable_t * p_htable)
{

    if (!p_htable)
    {
        return;
    }

    printf("\nHash Table: \n");

    for (uint32_t iter = 0; iter < p_htable->capacity; iter ++)
    {
        printf("%d: ", iter);

        if (NULL != p_htable->p_hash_vals[iter].p_value)
        {
            printf ("%s: ", (char *)((htable_node_t *)(p_htable->p_hash_vals[iter].p_value))->p_key);
            printf ("%s, ", ((char *)((htable_node_t *)p_htable->p_hash_vals[iter].p_value)->p_value));
            htable_node_t * p_curr_node = p_htable->p_hash_vals[iter].p_value;

            while (NULL != p_curr_node->p_next)
            {
                p_curr_node = p_curr_node->p_next;
                printf("%s: ", (char *)p_curr_node->p_key);
                printf("%s, ", ((char *)p_curr_node->p_value));

            }

            printf("\n");
        }

        else
        {
            printf ("Empty.\n");
        }
    }
}



uint32_t string_hash(void *p_data, const uint32_t size)

{
    uint32_t hash = 0;
    char *p_str = (char *)p_data;
    for (uint32_t i = 0; i < strnlen(p_str, 100); i++)
    {
        hash += p_str[i];
    }
    return hash % size;
}

int str_comp_f (const void * item1, const void * item2)
{
    char * str1 = (char *)item1;
    char * str2 = (char *)item2;
    int comp = strncmp(str1, str2, 100);
    return (comp);
}

int int_comp_f (const void * item1, const void * item2)
{
    return (*(int *)item1 - *(int *)item2);
}


void
list_free (void * p_list_void)
{
    list_t * p_list = (list_t *)p_list_void;
    list_node_t * p_node = p_list->p_head;
    list_node_t * p_temp = NULL;
    for (uint32_t idx = 0; idx < p_list->size; idx++)
    {
        p_temp = p_node->p_next;
        FREE(p_node);
        p_node = p_temp;
    }
    FREE(p_list);
}


int main()
{
    char * name1 = "emma";
    char * name2 = "jeremy";
    char * name3 = "jimmy";
    char * name4 = "jeremiah";
    char * name5 = "paul";
    char * name6 = "ben";
    // char * name12 = "Matthew";
    char * name7 = "Chris";
    // char * name8 = "Jeremiah";
    char * name9 = "Ryan";
    // char * name10 = "Derek";
    // char * name11 = "Adam";
    char * pass5 = "123";
    // char * pass6 = "321";
    // char * pass7 = "456";
    // char * pass8 = "654";
    // char * pass9 = "789";
    // char * pass10 = "987";
    // char * pass11 = "101010";
    int a = 1;

    list_t * p_list = list_create(int_comp_f);

    list_append(p_list, &a, NULL);

    htable_t * p_htable = htable_create(string_hash, str_comp_f);

    if (!p_htable)
    { 
        return 0;
    }

    htable_insert(&p_htable, name1, pass5, NULL);
    print_htable(p_htable);
    htable_insert(&p_htable, name2, pass5, NULL);
    print_htable(p_htable);
    htable_insert(&p_htable, name3, pass5, NULL);
    print_htable(p_htable);
    htable_insert(&p_htable, name4, pass5, NULL);
    print_htable(p_htable);
    htable_insert(&p_htable, name5, pass5, NULL);
    print_htable(p_htable);
    htable_insert(&p_htable, name6, pass5, NULL);
    print_htable(p_htable);
    htable_insert(&p_htable, name7, pass5, NULL);
    print_htable(p_htable);
    htable_insert(&p_htable, name9, pass5, NULL);
    print_htable(p_htable);
    htable_insert(&p_htable, &a, p_list, list_free);
    print_htable(p_htable);

    htable_destroy(&p_htable);
}


#endif

/* END OF FILE */

