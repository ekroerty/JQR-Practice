#ifndef _SERVER_H
#define _SERVER_H

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>

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
    char * name;
    uint32_t attack;
    uint32_t dodge;
    uint32_t luck;
    int32_t health;

} fighter_t;

#endif