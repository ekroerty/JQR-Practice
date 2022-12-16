#ifndef _SERVER_H
#define _SERVER_H

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct server_t 
{
    int32_t domain;
    int32_t protocol;
    int32_t service;
    uint32_t interface;
    uint32_t port;
    uint32_t backlog;
    char * ip;
    struct sockaddr_in addr;
    int32_t socket;

} server_t;

typedef struct fighter_t
{
    char name[10];
    uint8_t attack;
    uint8_t dodge;
    uint8_t luck;
    int32_t health;

} fighter_t;

typedef struct file_t 
{
    FILE * file;
    char * type;
    char * ip;
    int port;
    char * status;
    struct tm * p_tm;
} file_t;

typedef struct fight_data_t
{
    fighter_t ** pp_fighter;
    int32_t *    client_fds;
    file_t *     p_file_struct;
} fight_data_t;

#endif