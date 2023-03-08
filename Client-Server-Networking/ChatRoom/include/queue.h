/**
 * @file queue.h
 * @author Emma Roerty
 * @brief Header file for queue data structure
 * @version 1.1
 * @date 2022-12-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _QUEUE_H
#define _QUEUE_H
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief user defined free function
*/

typedef void
(* FREE_F) (void * );

/**
 * @brief represents an element in a queue
 * 
 * @param p_data what data the queue node holds
 * @param p_next points to the next node in the queue
 * @param free function given by the user if the data is a
 *              complex data structure
 */

typedef struct queue_node_t
{
    void *                  p_data;
    struct queue_node_t *   p_next;
    FREE_F                  free_f;
} queue_node_t;

/**
 * @brief queue data structure, queue context
 * 
 * @param p_head pointer to the first element in the queue, index 0
 * @param p_tail pointer to the last element in the queue, the one that would be dequeued
 * @param size size of the queue 
 */

typedef struct queue_t
{
    queue_node_t * p_head;
    queue_node_t * p_tail;
    uint32_t       size;
} queue_t;

/**
 * @brief creates a queue
 * 
 * @return returns queue instance
 */

queue_t *
queue_create ();

/**
 * @brief returns size of queue using already defined attribute
 * 
 * @param p_queue supplies a queue for the function to reference
 * 
 * @return returns queue size or -1 on error
 * 
 */

int32_t
queue_size (queue_t *);

/**
 * @brief returns boolean to test if the queue is empty
 * 
 * @param p_queue supplies a queue for the function to test on 
 * 
 * @return returns true if the queue is empty, returns false if the queue is not empty
 * 
 */

bool
queue_is_empty (queue_t *);

/**
 * @brief adds a node to the end of the queue
 * 
 * @param p_queue defines what queue to add on to
 * @param p_data the data that the user wants to enqueue
 * @param free_f free function given by the user if the data is a
 *               complex data structure
 * 
 * @return true if successful, false if not
 * 
 */

bool
queue_enqueue (queue_t *, void *, FREE_F);

/**
 * @brief removes a node from the end of the queue
 * 
 * @param p_queue defines what queue to remove a node from
 * 
 * @return the data within the node being dequeued, or null if unsuccessful
 * 
 */

void *
queue_dequeue (queue_t *);

/**
 * @brief can peek at the head of the queue
 * 
 * @param p_queue queue for the function to use
 * 
 * @return the data that the head node contains
 * 
 */

void *
queue_peek (queue_t *);

/**
 * @brief can destroy items in a queue 
 * 
 * @param p_queue queue to remain at memory location, but it will be empty
 * 
 * @return true if successful, false if not
 */

bool
queue_destroy_items (queue_t *);

/**
 * @brief can destroy a queue context
 * 
 * @param p_queue queue to be destroyed
 * 
 * @return true if successful, false if not
 * 
 */

bool
queue_destroy (queue_t **);

#endif

/* END OF FILE */
