#include "../include/queue.h"
#include "../include/free.h"
#include <stdlib.h>
#include <stdio.h>

#define SUCCESS true
#define Q_FAILURE NULL
#define B_FAILURE false

queue_t *
queue_create ()
{
    queue_t * p_queue = calloc(1, sizeof(queue_t));

    if (NULL == p_queue)
    {
        return Q_FAILURE;
    }

    p_queue->p_head = NULL;
    p_queue->p_tail = NULL;
    p_queue->size = 0;
    return p_queue;
}

int32_t
queue_size (queue_t * p_queue)
{
    
    if (NULL == p_queue)
    {
        return B_FAILURE;
    }

    return p_queue->size;    
}

bool
queue_is_empty (queue_t * p_queue)
{

    if (NULL == p_queue)
    {
        return SUCCESS;
    }

    if (0 == queue_size(p_queue))
    {
        return SUCCESS;
    }

    return B_FAILURE;    
}

bool
queue_enqueue (queue_t * p_queue, void * p_data, FREE_F free_f)
{

    if ((NULL == p_data) ||(NULL == p_queue))
    {
        return B_FAILURE;
    }

    queue_node_t * p_node = calloc(1, sizeof(queue_node_t));

    if (NULL == p_node)
    {
        return B_FAILURE;
    }

    p_node->p_data = p_data;
    p_node->position = p_queue->size;
    p_node->p_next = NULL;
    p_node->free_f = free_f;

    if (queue_is_empty(p_queue))
    {
        p_queue->p_head = p_node;
        p_queue->p_tail = p_node;
        p_queue->size++;
    }
    else
    {
        p_queue->size++;
        p_queue->p_tail->p_next = p_node;
        p_queue->p_tail = p_node;
    }

    return SUCCESS;
}


void *
queue_dequeue (queue_t * p_queue)
{
    queue_node_t * p_temp = NULL;
    void *         p_val = NULL;

    if (NULL == p_queue)
    {
        return NULL;
    }

    if (queue_is_empty(p_queue))
    {
        return Q_FAILURE;
    }

    p_val = p_queue->p_head->p_data;
    p_temp = p_queue->p_head;
    p_queue->p_head->p_data = NULL;
    p_queue->p_head = p_queue->p_head->p_next;
    p_queue->size--;

    // if (p_temp->free_f)
    // {
    //     p_temp->free_f(p_temp->p_data);
    // }

    FREE(p_temp);
    return p_val;
}

void *
queue_peek (queue_t * p_queue)
{
    void * p_val = NULL;

    if (NULL == p_queue)
    {
        return Q_FAILURE; 
    }

    if (queue_is_empty(p_queue))
    {
        return Q_FAILURE;
    }

    p_val = p_queue->p_head->p_data;
    return p_val;
}


bool
queue_destroy_items (queue_t * p_queue)
{

    if (NULL == p_queue)
    {
        return B_FAILURE;
    }

    if (queue_is_empty(p_queue))
    {
        return SUCCESS;
    }

    queue_node_t * p_node = p_queue->p_head;
    queue_node_t * p_temp = NULL;

    for (uint32_t iter = 0; iter < p_queue->size; iter++)
    {
        p_temp = p_node;
        p_node = p_node->p_next;

        if (NULL != p_temp->free_f) 
        { 
            p_temp->free_f(p_temp->p_data); 
        }

        FREE(p_temp);
    }

    p_queue->p_head = NULL;
    p_queue->p_tail = NULL;
    p_queue->size = 0;
    return SUCCESS;   
}

bool
queue_destroy (queue_t ** pp_queue)
{
    bool des;

    if ((NULL == *pp_queue) || (NULL == pp_queue))
    {
        return B_FAILURE;
    }

    des = queue_destroy_items(*pp_queue);

    if (!des)
    {
        return B_FAILURE;  
    }

    FREE(*pp_queue);
    pp_queue = NULL;
    return SUCCESS;
}
