#ifndef _QUEUE_H
#define _QUEUE_H
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief user defined free function
 * 
*/

typedef void
(* FREE_F) (void * );

/**
 * @brief represents an element in a queue
 * 
 * @param data what the element is (ie. a number)
 * @param position where the node is within the queue (ie. it's index)
 * @param next points to the next
 */

typedef struct queue_node_t
{
    struct queue_node_t *   p_next;
    void *                  p_data;
    uint32_t                position;
    FREE_F                  free_f;
} queue_node_t;

/**
 * @brief queue data structure, creates a queue context
 * 
 * @param head pointer to the first element in the queue, index 0
 * @param tail pointer to the last element in the queue, the one that would be dequeued
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
 * 
 */

queue_t *
queue_create ();

/**
 * @brief returns size of queue using already defined attribute
 * 
 * @param queue supplies a queue for the function to reference
 * 
 * @return returns queue size
 * 
 */

int32_t
queue_size (queue_t *);

/**
 * @brief returns boolean to test if the queue is empty
 * 
 * @param queue supplies a queue for the function to test on 
 * @return returns true if the queue is empty, returns false if the queue is not empty
 * 
 */

bool
queue_is_empty (queue_t *);

/**
 * @brief adds a node to the end of the queue
 * 
 * @param queue defines what queue to add on to
 * @param data the data that the user wants to enqueue
 * 
 * @return 1 if successful, -1 if not
 * 
 */

bool
queue_enqueue (queue_t *, void *, FREE_F);

/**
 * @brief removes a node from the front of the queue
 * 
 * @param queue defines what queue to remove a node from
 * 
 * @return the data within the node being dequeued, or -1 if unsuccessful
 * 
 */

void *
queue_dequeue (queue_t *);

/**
 * @brief can peek at the head of the queue
 * 
 * @param queue queue for the function to use
 * 
 * @return the data that the head node contains
 * 
 */

void *
queue_peek (queue_t *);

/**
 * @brief can destroy items in a queue 
 * 
 * @param queue queue to remain at memory location, but it will be empty
 * 
 * @return 1 if successful, -1 if not
 */

bool
queue_destroy_items (queue_t *);

/**
 * @brief can destroy a queue context
 * 
 * @param queue queue to be destroyed
 * 
 * @return return 1 if successful, 0 if failure
 * 
 */

bool
queue_destroy (queue_t **);

#endif