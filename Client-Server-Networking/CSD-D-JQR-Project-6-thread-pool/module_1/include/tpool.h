#ifndef _TPOOL_H
#define _TPOOL_H

#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>

#include "./queue.h"


/**
 * @brief user defined free function
 * 
*/

typedef void
(* FREE_F) (void *);

/**
 * @brief user-defined job function
*/

typedef void
(*JOB_F) (void *);

/**
 * @brief job structure to be enqueued
 * 
 * @param job_function that the user provides
 * @param p_args void pointer to one or more arguments
 * @param free_f user defined free function for given arguments
 * 
*/

typedef struct tpool_job_t 
{
    JOB_F    job_f;
    void *   p_args;
    FREE_F   free_f;

} tpool_job_t;


/**
 * @brief thread pool data type
 * 
 * @param p_thread_arr array of threads based on the number of threads the user specifies
 * @param p_job_queue queue holding the jobs
 * @param mutex locks the shared resource, which is the job queue
 * @param cond lets us know if we should exit or not
 * @param size the number of threads the user specifies
*/

typedef struct tpool_t
{
    pthread_t *     p_thread_arr;
    queue_t *       p_job_queue;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    uint32_t        thread_count;
    _Atomic bool    b_run;

} tpool_t;

/**
 * @brief can create a thread pool context
 * 
 * @param n_threads number of threads the user wants to initialize
 * 
 * @return context
*/

tpool_t *
tpool_create(uint32_t);

/**
 * @brief can add jobs to the job queue
 * 
 * @param p_tpool instance to add job to
 * @param job_f function to insert into the queue
 * @param p_args arguments that the function will take
 * @param free_f free function for the data structure that is p_args
 * 
 * @return 1 if successful, -1 if not0
 * 
*/

bool
tpool_add_job(tpool_t *, JOB_F, void *, FREE_F);

/**
 * @brief can destroy thread pool context
 * 
 * @param p_tpool instance to destroy. should shut down threads, destroy the queue, and free the tpool
 * 
 * @return 1 if successful, -1 if not
*/

bool
tpool_destroy(tpool_t **);


#endif