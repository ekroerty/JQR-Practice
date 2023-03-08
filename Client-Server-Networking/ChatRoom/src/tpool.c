/**
 * @file tpool.c
 * @author Emma Roerty
 * @brief Source file for thread pool data structure
 * @version 1.1
 * @date 2022-12-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../include/tpool.h"
#include "../include/free.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define SUCCESS true
#define TP_FAILURE NULL
#define N_FAILURE false

static bool
handle_threads (tpool_t *);

static void *
handle_work (void *);

static void
execute_job (tpool_job_t *);

static void
shutdown_threads (tpool_t *);

tpool_t *
tpool_create (uint32_t thread_count)
{
    tpool_t * p_tpool = calloc(1, sizeof(tpool_t));

    if (!p_tpool)
    {
        return TP_FAILURE;
    }

    p_tpool->thread_count = thread_count;
    p_tpool->p_thread_arr = calloc(thread_count, sizeof(pthread_t));
    p_tpool->b_run = true;

    if (!p_tpool->p_thread_arr)
    {
        FREE(p_tpool);
        return TP_FAILURE;
    }

    p_tpool->p_job_queue = queue_create();

    if (!p_tpool->p_job_queue)
    {
        FREE(p_tpool->p_thread_arr);
        FREE(p_tpool);
        return TP_FAILURE;
    }

    pthread_mutex_init(&p_tpool->mutex, NULL);
    pthread_cond_init(&p_tpool->cond, NULL);
    bool ret = handle_threads(p_tpool);

    if (ret)
    {
        return p_tpool;
    }

    return TP_FAILURE;
}

bool
tpool_add_job (tpool_t * p_tpool, JOB_F job_f, void * p_args, FREE_F free_f)
{

    if ((!p_tpool) || (!job_f))
    {
        return N_FAILURE;
    }

    tpool_job_t * job_func = calloc(1, sizeof(tpool_job_t));
    
    if (!job_func)
    {
        return N_FAILURE;
    }

    job_func->job_f = job_f;
    job_func->p_args = p_args;
    job_func->free_f = free_f;

    pthread_mutex_lock(&p_tpool->mutex);
    queue_enqueue(p_tpool->p_job_queue, job_func, free_f);
    pthread_mutex_unlock(&p_tpool->mutex);
    pthread_cond_broadcast(&p_tpool->cond);
    return SUCCESS;
}

static bool
handle_threads (tpool_t * p_tpool)
{
    int         create_status;
    uint32_t    counter = 0;
    pthread_t * p_thread;

    for (uint32_t iter = 0; iter < p_tpool->thread_count; iter++)
    {
        p_thread = &(p_tpool->p_thread_arr[iter]);
        create_status = 
                pthread_create(p_thread, NULL, handle_work, (void *)p_tpool);

        if (0 == create_status)
        {
            counter++;
        }
        else
        {
            p_tpool->b_run = false;
            p_tpool->thread_count = counter;
            tpool_destroy(&p_tpool);
            return N_FAILURE;
        }
    
    }

    return SUCCESS;
}


static void *
handle_work (void * p_tpool_void)
{
    tpool_t *     p_tpool = (tpool_t *)p_tpool_void;
    tpool_job_t * p_job = NULL;
 
    for (;;)
    {
        pthread_mutex_lock(&p_tpool->mutex);
        
        while (queue_is_empty(p_tpool->p_job_queue) && p_tpool->b_run)
        {
            pthread_cond_wait(&p_tpool->cond, &p_tpool->mutex);
        }

        if (queue_is_empty(p_tpool->p_job_queue) && !p_tpool->b_run)
        {
            pthread_mutex_unlock(&p_tpool->mutex);
            break;
        }

        p_job = (tpool_job_t *)queue_dequeue(p_tpool->p_job_queue);
        pthread_mutex_unlock(&p_tpool->mutex);
        execute_job(p_job);
    }
    
    return NULL;
}

static void 
execute_job (tpool_job_t * p_job)
{

    if (p_job)
    {
        p_job->job_f(p_job->p_args);

        if (p_job->free_f)
        {
            p_job->free_f(p_job->p_args);
        }

        FREE(p_job);
    }

}

static void
shutdown_threads (tpool_t * p_tpool)
{

    if (!p_tpool->p_thread_arr)
    {
        return;
    }

    // pthread_mutex_lock(&p_tpool->mutex);
    pthread_cond_broadcast(&p_tpool->cond);
    // pthread_mutex_unlock(&p_tpool->mutex);

    for (uint32_t iter = 0; iter < p_tpool->thread_count; iter++)
    {
        pthread_join(p_tpool->p_thread_arr[iter], NULL);
    }

    return;
}

bool
tpool_destroy (tpool_t ** pp_tpool)
{

    if ((!pp_tpool) || (!*pp_tpool))
    {
        return N_FAILURE;
    }

    (*pp_tpool)->b_run = false;
    shutdown_threads(*pp_tpool);
    FREE((*pp_tpool)->p_thread_arr);
    pthread_mutex_destroy(&(*pp_tpool)->mutex);
    pthread_cond_destroy(&(*pp_tpool)->cond);
    queue_destroy(&(*pp_tpool)->p_job_queue);
    FREE(*pp_tpool);
    pp_tpool = NULL;
    return SUCCESS;
}


