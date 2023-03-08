/**
 * @file list.c
 * @author Emma Roerty
 * @brief Source file for list data structure
 * @version 1.1
 * @date 2022-12-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/list.h"
#include "../include/free.h"
#include <stdlib.h>
#include <stdio.h>

#define SUCCESS true
#define L_FAILURE NULL
#define B_FAILURE false
#define N_FAILURE -1

void
print_list(list_t *);

static list_node_t * 
merge(list_node_t *, list_node_t *, list_node_t **, CMP_F);

static list_node_t *
node_merge_sort(list_node_t *, uint32_t, list_node_t **, CMP_F);

list_t *
list_create (CMP_F comp_f)
{
    list_t * p_cll = calloc(1, sizeof(list_t));

    if (NULL == p_cll)
    {
        return L_FAILURE;
    }

    p_cll->p_head = NULL;
    p_cll->p_tail = NULL;
    p_cll->size = 0;
    p_cll->comp_f = comp_f;
    return p_cll;
}

int32_t
list_size (list_t * p_cll)
{

    if (NULL == p_cll)
    {
        return B_FAILURE;
    }

    return p_cll->size;
}

bool
list_append (list_t * p_cll, void * p_data, FREE_F free_f)
{

    if ((NULL == p_cll) || (NULL == p_data))
    {
        return B_FAILURE;
    }

    list_node_t * p_node = calloc(1, sizeof(list_node_t));

    if (NULL == p_node)
    {
        return B_FAILURE;
    }

    p_node->p_data = p_data;
    p_node->free_f = free_f;

    if (0 == list_size(p_cll))
    {
        p_cll->p_head = p_node;
        p_cll->p_tail = p_node;
        p_cll->size++;
        p_node->p_next = p_cll->p_head;
    }
    else
    {
        p_cll->p_tail->p_next = p_node;
        p_cll->p_tail = p_node;
        p_node->p_next = p_cll->p_head;
        p_cll->size++;
    }

    return SUCCESS;
}

bool
list_prepend (list_t * p_cll, void * p_data, FREE_F free_f)
{

    if ((NULL == p_data) || (NULL == p_cll))
    {
        return B_FAILURE;
    }

    list_node_t * p_node = calloc(1, sizeof(list_node_t));

    if (NULL == p_node)
    {
        return B_FAILURE;
    }

    p_node->p_data = p_data;
    p_node->free_f = free_f;

    if (0 == list_size(p_cll))
    {
        p_cll->p_head = p_node;
        p_cll->p_tail = p_node;
        p_cll->size++;
        p_node->p_next = p_cll->p_head;
    }
    else
    {
        p_node->p_next = p_cll->p_head;
        p_cll->p_head = p_node;
        p_cll->p_tail->p_next = p_cll->p_head;
        p_cll->size++;
    }

    return SUCCESS;
}

bool
list_insert (list_t * p_cll, void * p_data, uint32_t idx, FREE_F free_f)
{

    if ((NULL == p_cll) || (NULL == p_data))
    {
        return B_FAILURE;
    }

    list_node_t * p_curr_node = p_cll->p_head;

    if ((0 == idx) || (NULL == p_curr_node))
    {
        return list_prepend(p_cll, p_data, free_f);
    }

    if ((p_cll->size) <= idx)
    {
        return list_append(p_cll, p_data, free_f);
    }

    list_node_t * p_node = calloc(1, sizeof(list_node_t));

    if (NULL == p_node)
    {
        return B_FAILURE;
    }

    p_node->p_data = p_data;
    p_node->free_f = free_f;
    p_node->p_next = NULL;
    
    for (uint32_t iter = 0; iter < p_cll->size; iter++)
    {

        if (idx == (iter + 1))
        {
            p_node->p_next = p_curr_node->p_next;
            p_cll->size++;
            break;
        }

        p_curr_node = p_curr_node->p_next;
    }

    p_curr_node->p_next = p_node;
    return SUCCESS;
}

void *
list_pop_tail (list_t * p_cll)
{

    void *        p_val = NULL;
    list_node_t * p_temp = NULL;

    if ((NULL == p_cll) || (0 == list_size(p_cll)))
    {
        return L_FAILURE;
    }

    list_node_t * p_curr_node = p_cll->p_head;

    for (uint32_t iter = 0; iter < p_cll->size; iter++)
    {

        if (p_curr_node->p_next == p_cll->p_tail)
        {
            p_val = p_cll->p_tail->p_data;
            p_temp = p_cll->p_tail;
            p_cll->p_tail = p_curr_node;
            p_curr_node->p_next = p_cll->p_head;
            p_cll->size--;
            p_temp->p_data = NULL;
            FREE(p_temp);
            break;
        }

        p_curr_node = p_curr_node->p_next;
    }

    return p_val;
}

void *
list_pop_head (list_t * p_cll)
{

    if ((NULL == p_cll) || (0 == list_size(p_cll)))
    {
        return L_FAILURE;
    }

    void *        p_val = p_cll->p_head->p_data;
    list_node_t * p_temp = NULL;

    p_temp = p_cll->p_head;
    p_cll->p_head = p_cll->p_head->p_next;
    p_cll->p_tail->p_next = p_cll->p_head;
    p_cll->size--;
    FREE(p_temp);
    return p_val;
}

void * 
list_remove (list_t * p_cll, uint32_t idx)
{
    void *        p_val = NULL;
    list_node_t * p_temp = NULL;

    if ((NULL == p_cll) || (0 == list_size(p_cll)))
    {
        return L_FAILURE;
    }

    if (0 == idx)
    {
        return list_pop_head(p_cll);
    }

    if (idx == (p_cll->size - 1))
    {
        return list_pop_tail(p_cll);
    }

    if (p_cll->size <= idx)
    {
        return L_FAILURE;
    }

    list_node_t * p_curr_node = p_cll->p_head;

    for (uint32_t iter = 0; iter < p_cll->size; iter++)
    {

        if (idx == (iter + 1))
        {
            p_val = p_curr_node->p_next->p_data;
            p_temp = p_curr_node->p_next;
            p_curr_node->p_next = p_curr_node->p_next->p_next;
            p_cll->size--;
            p_temp->p_next = NULL;
            p_temp->p_data = NULL;
            FREE(p_temp);
            break;
        }
        else
        {
            p_curr_node = p_curr_node->p_next;
        }

    }

    p_curr_node = NULL;
    return p_val;
}

void *
list_index(list_t * p_cll, uint32_t idx)
{
    void * p_val = NULL;

    if ((NULL == p_cll) || (idx >= p_cll->size))
    {
        return L_FAILURE;
    }

    list_node_t * p_curr_node = p_cll->p_head;

    for (uint32_t iter = 0; iter <= idx; iter++)
    {

        if (idx == iter)
        {
            p_val = p_curr_node->p_data;
        }

        p_curr_node = p_curr_node->p_next;
    }

    return p_val;
}

int32_t
list_first_occurrence (list_t * p_cll, void * p_item)
{

    if ((NULL == p_cll) || (NULL == p_item) || (NULL == p_cll->comp_f))
    {
        return N_FAILURE;
    }

    list_node_t * p_curr_node = p_cll->p_head;
    void *        p_val = NULL;
    int           comp;

    for (uint32_t iter = 0; iter < p_cll->size; iter++)
    {
        p_val = p_curr_node->p_data;
        comp = p_cll->comp_f(p_val, p_item);

        if (0 == comp)
        {
            return iter;
        }
        else
        {
            p_curr_node = p_curr_node->p_next;
        }

    }

    return N_FAILURE;
}

void *
list_remove_val (list_t * p_cll, void * p_item)
{

    if ((NULL == p_cll) || (NULL == p_item) || (NULL == p_cll->comp_f))
    {
        return L_FAILURE;
    }

    list_node_t * p_curr_node = p_cll->p_head;
    list_node_t * p_rm_node = NULL;
    void *        p_val = NULL;
    int           comp;

    if (0 == p_cll->comp_f(p_cll->p_head->p_data, p_item))
    {
        return list_pop_head(p_cll);
    }
    if (0 == p_cll->comp_f(p_cll->p_tail->p_data, p_item))
    {
        return list_pop_tail(p_cll);
    }
    for (uint32_t iter = 0; iter < p_cll->size-1; iter++)
    {
        p_val = p_curr_node->p_next->p_data;
        comp = p_cll->comp_f(p_val, p_item);

        if (0 == comp)
        {
            p_rm_node = p_curr_node->p_next;
            p_curr_node->p_next = p_curr_node->p_next->p_next;
            p_cll->size--;
            p_rm_node->p_data = NULL;
            p_rm_node->p_next = NULL;
            FREE(p_rm_node);
            return p_val;
        }
        else
        {
            p_curr_node = p_curr_node->p_next;
        }

    }

    return L_FAILURE;
}

list_node_t * 
merge (list_node_t * p_left, list_node_t * p_right,
                                list_node_t ** pp_tail, CMP_F comp_f)
{
    list_node_t * p_curr_node = NULL;

    if (NULL == p_left)
    {
        p_curr_node = p_right;

        while (p_curr_node->p_next)
        {
            p_curr_node = p_curr_node->p_next;
        }

        *pp_tail = p_curr_node;
        return p_right;
    }
     if (NULL == p_right)
    {
        p_curr_node = p_left;

        while (p_curr_node->p_next)
        {
            p_curr_node = p_curr_node->p_next;
        }

        *pp_tail = p_curr_node;
        return p_left;
    }

    if (0 >= comp_f(p_left->p_data, p_right->p_data))
    {
        p_left->p_next = merge(p_left->p_next, p_right, pp_tail, comp_f);
        return p_left;
    }

    p_right->p_next = merge(p_right->p_next, p_left, pp_tail, comp_f);
    return p_right;
}

bool
list_merge_sort (list_t * p_cll)
{

    if ((NULL == p_cll) || (NULL == p_cll->p_head) || (NULL == p_cll->comp_f))
    {
        return B_FAILURE;
    }

    list_node_t * p_tail_node = NULL;
    p_cll->p_tail->p_next = NULL;
    p_cll->p_head = node_merge_sort
                    (p_cll->p_head, p_cll->size, &p_tail_node, p_cll->comp_f);

    if (NULL == p_cll->p_head)
    {
        return B_FAILURE;
    }

    if (p_tail_node)
    {
        p_cll->p_tail = p_tail_node;
        p_cll->p_tail->p_next = p_cll->p_head;
        return SUCCESS;
    }

    return B_FAILURE;
}

list_node_t *
node_merge_sort 
    (list_node_t * p_node, uint32_t size, list_node_t ** pp_tail, CMP_F cmp)
{

    if (NULL == p_node)
    {
        return L_FAILURE;
    }

    if (1 == size)
    {
        return p_node;
    }

    list_node_t * p_left = p_node;
    list_node_t * p_right = NULL;
    list_node_t * p_curr_node = p_node;
    list_node_t * p_left_tail = NULL;
    list_node_t * p_right_tail = NULL;
    uint32_t      half_size = (size / 2);
    uint32_t      p_right_size = (size - half_size);

    for (uint32_t iter = 0; iter < size; iter++)
    {

        if (size == (iter + 1))
        {
            p_right_tail = p_curr_node;
            *pp_tail = p_right_tail;
            p_right_tail->p_next = NULL;
            p_curr_node->p_next = NULL;
            break;
        }

        if (iter < (half_size - 1))
        {
            p_curr_node = p_curr_node->p_next;
            continue;
        }
        else if (half_size == (iter + 1))
        {
            p_right = p_curr_node->p_next;
            p_left_tail = p_curr_node;
            p_curr_node = p_curr_node->p_next;
            p_left_tail->p_next = NULL;
            continue;
        }
        else
        {
            p_curr_node = p_curr_node->p_next;
        }

    }

    p_left = node_merge_sort(p_left, half_size, pp_tail, cmp);
    p_right = node_merge_sort(p_right, p_right_size, pp_tail, cmp);
    return merge(p_left, p_right, pp_tail, cmp);
}

bool
list_destroy_items (list_t * p_cll)
{

    if (NULL == p_cll)
    {
        return B_FAILURE;
    }

    if (0 == list_size(p_cll))
    {
        return SUCCESS;
    }

    list_node_t * p_node = p_cll->p_head;
    list_node_t * p_temp = NULL;

    for (uint32_t iter = 0; iter < p_cll->size; iter++)
    {
        p_temp = p_node;
        p_node = p_node->p_next;

        if (p_temp->free_f)
        {
            p_temp->free_f(p_temp->p_data);
        }

        FREE(p_temp);
    }

    p_cll->p_head = NULL;
    p_cll->p_tail = NULL;
    p_cll->size = 0;
    return SUCCESS;
}

bool
list_destroy (list_t ** pp_cll)
{

    if ((NULL == *pp_cll) || (NULL == pp_cll))
    {
        return B_FAILURE;
    }

    int32_t des = list_destroy_items(*pp_cll);

    if (!des)
    {
        return B_FAILURE;
    }

    FREE(* pp_cll);
    pp_cll = NULL;
    return SUCCESS;
}

#if 0
void print_list (list_t * p_cll)
{
    list_node_t *node = p_cll->p_head;
    printf("list: ");
    for (uint32_t i = 0; i < p_cll->size; i++)
    {
        printf("%d ", *(int *)node->p_data);
        node = node->p_next;
    }
    printf("\n");
}
#endif

/* END OF FILE */
