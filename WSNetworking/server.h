#ifndef _SERVER_H
#define _SERVER_H

#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct server_t
{
    int domain;
    int protocol;
    int service;
    uint32_t interface;
    int port;
    int backlog;
    int socket;
    struct sockaddr_in address;
    void (*launch)(struct server_t *);

} server_t;

server_t 
server_create(int domain, int protocol, int service, uint32_t interface,
                            int port, int backlog, void (*launch)(struct server_t *));



#endif