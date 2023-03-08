#include "../include/server.h"
#include <fcntl.h>
#include <signal.h>

#define PROTOCOL 0
#define CLIENT_PORT 5555
#define BACKLOG 100
#define IP "127.0.0.1"

#define N_FAILURE -1

#define THREADS 10

_Atomic bool gb_run = true;

void object_teardown(objects_t *, int);

void
sighandler(int signum)
{
    printf("\nCaught signal %d. Gracefully exiting...\n", signum);
    gb_run = false;
}

void
conn_data_free(void * p_conn_arg)
{
    conn_data_t * p_conn = p_conn_arg;
    FREE(p_conn);
}

int
sess_comp(const void * p_client1, const void * session_id)
{
    if (!p_client1)
    {
        return N_FAILURE;
    }
    client_t * p_client = (client_t *)p_client1;
    uint64_t   id       = (uint64_t) session_id;
    return (p_client->session_id - id);
}

server_t
create_server ()
{
    int enable = 1;
    server_t server;

    server.domain = AF_INET;
    server.protocol = PROTOCOL;
    server.service = SOCK_STREAM;
    server.port = CLIENT_PORT;
    server.backlog = BACKLOG;
    server.ip = IP;
    server.socket = socket(server.domain, server.service, server.protocol);

    memset(&server.addr, '\0', sizeof(server.addr));
    server.addr.sin_family = server.domain;
    server.addr.sin_port = htons(server.port);
    server.addr.sin_addr.s_addr = inet_addr(server.ip);

    setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    if (0 > server.socket)
    {
        printf("Unable to create socket. Exiting...\n");
        exit(1);
    }
    int bind_stat = bind(server.socket, 
                    (struct sockaddr *)&server.addr, sizeof(server.addr));

    if (0 > bind_stat)
    {
        exit(1);
    }

    printf("Server binded!\n");

    int listen_stat = listen(server.socket, server.backlog);

    if (0 > listen_stat)
    {
        exit(1);
    }

    printf("Server listening!\n");

    return server;

}

objects_t * object_init(int max_clients)
{
    if (!max_clients)
    {
        return NULL;
    }

    objects_t * p_objects = calloc(1, sizeof(objects_t));

    if (!p_objects)
    {
        return NULL;
    }

    p_objects->pb_run = &gb_run;

    const char * p_log_file = "log.txt";
    FILE * p_log = fopen(p_log_file, "w");

    if (!p_log)
    {
        object_teardown(p_objects, 2);
        return NULL;
    }

    fclose(p_log);

    p_objects->p_log = p_log_file;
    p_objects->session_id = 0;
    p_objects->p_htable = NULL;

    signals_t * p_signals = calloc(1, sizeof(signals_t));

    if (!p_signals)
    {
        object_teardown(p_objects, 2);
        return NULL;
    }

    p_objects->p_signals = p_signals;

    int success = pthread_mutex_init(&p_objects->p_signals->client_mutex, NULL);
    
    if (0 > success)
    {
        object_teardown(p_objects, 1);
        return NULL;
    }

    success = pthread_mutex_init(&p_objects->p_signals->sess_mutex, NULL);
    
    if (0 > success)
    {
        object_teardown(p_objects, 4);
        return NULL;
    }

    success = pthread_mutex_init(&p_objects->p_signals->log_mutex, NULL);

    if (0 > success)
    {
        object_teardown(p_objects, 5);
        return NULL;
    }

    success = pthread_cond_init(&p_objects->p_signals->cond, NULL);

    if (0 > success)
    {
        object_teardown(p_objects, 6);
        return NULL;
    }


    return p_objects;
}

void object_teardown(objects_t * p_objects, int opt)
{
    switch (opt)
    {
        case 7:
        {
            pthread_cond_broadcast(&p_objects->p_signals->cond);
            pthread_cond_destroy(&p_objects->p_signals->cond);
            __attribute__((fallthrough));

        }
        case 6:
        {
            pthread_mutex_destroy(&p_objects->p_signals->log_mutex);
            __attribute__((fallthrough));
        }
        case 5:
        {
            pthread_mutex_destroy(&p_objects->p_signals->sess_mutex);
            __attribute__((fallthrough));
        }
        case 4:
        {
            pthread_mutex_destroy(&p_objects->p_signals->client_mutex);
            __attribute__((fallthrough));
        }
        case 1:
        {
            FREE(p_objects->p_signals);
            __attribute__((fallthrough));
        }
        case 2:
        {
            FREE(p_objects);
        }
    }
}

int main ()
{
    struct sockaddr_in client_addr;

    server_t server = create_server();
    socklen_t addr_size = sizeof(&client_addr);
    fcntl(server.socket, F_SETFL, O_NONBLOCK);
    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);
    signal(SIGQUIT, sighandler);
    signal(SIGTSTP, sighandler);

    objects_t * p_objects = object_init(server.backlog);

    if (!p_objects)
    {
        return 0;
    }

    conn_data_t * p_conn = NULL;

    tpool_t * p_tpool = tpool_create(THREADS);
    bool client_job;

    printf("[LISTENING] TCP Port %d\n", CLIENT_PORT);
    int client_sock;

    while (*p_objects->pb_run)
    {
        client_sock = accept(server.socket, 
                                (struct sockaddr *)&client_addr,
                                &addr_size);

        if (0 > client_sock)
        {
            continue;
        }

        p_conn = calloc(1, sizeof(conn_data_t));

        if (!p_conn)
        {
            continue;
        }
        
        p_conn->fd = client_sock;
        p_conn->p_addr = &client_addr;
        p_conn->p_objects = p_objects;
        client_job = tpool_add_job(p_tpool, (JOB_F)handle_clients, 
                     (void *)p_conn, conn_data_free);

        if (!client_job)
        {
            conn_data_free(p_conn);
        }

    }
    // conn_data_free(p_conn);
    // close(client_sock);
    // printf("")
    htable_destroy(&p_objects->p_htable);
    pthread_cond_broadcast(&p_objects->p_signals->cond);
    tpool_destroy(&p_tpool);
    object_teardown(p_objects, 7);
    return 0;
}