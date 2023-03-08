#ifndef _SERVER_H
#define _SERVER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include "./list.h"
#include "./free.h"
#include "./tpool.h"
#include "./htable.h"
#include "./packets.h"

#define MSG 0x03
#define DISCONNECT 0x04
#define DELETE 0x05
#define UPDATE 0x06
#define FILE_UPLOAD 0x07
#define FILE_DOWNLOAD 0x08

#define INIT 0x01
#define ACKNOWLEDGE 0x11
#define FAILURE 0xFF
#define SERVER_ERROR 0x06
#define INVALID_UNAME 0x07
#define INVALID_LOGIN 0x08
#define BUFF_SIZE 1024


typedef struct server_t 
{
    int32_t            domain;
    int32_t            protocol;
    int32_t            service;
    uint32_t           interface;
    uint32_t           port;
    uint32_t           backlog;
    char *             ip;
    int32_t            socket;
    struct sockaddr_in addr;

} server_t;

typedef struct signals_t
{
    pthread_mutex_t client_mutex;
    pthread_mutex_t sess_mutex;
    pthread_mutex_t log_mutex;
    pthread_cond_t  cond;
} signals_t;

typedef struct objects_t
{
    list_t       * p_clients;
    const char   * p_log;
    uint64_t       session_id;
    signals_t    * p_signals;
    htable_t     * p_htable;
    htable_t     * p_db;
    _Atomic bool * pb_run;
} objects_t;

typedef struct client_t
{
    uint64_t session_id;
    struct sockaddr_in * p_addr;
    char p_username[UNAME_MAX];
    char p_password[PASS_MAX];
    int fd;
} client_t;

typedef struct conn_data_t
{
    objects_t          * p_objects;
    struct sockaddr_in * p_addr;
    int fd;
} conn_data_t;

void
handle_clients(conn_data_t *);

void 
send_fail_pack (conn_data_t *, int);

#endif