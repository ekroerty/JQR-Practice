#include "./server.h"
#include <stdio.h>
#include <string.h>

server_t server_create(int domain, int protocol, int service, uint32_t interface,
                            int port, int backlog, 
                                            void (*launch)(server_t *))
    {
        server_t server;
        
        server.domain = domain;
        server.protocol = protocol;
        server.service = service;
        server.interface = interface;
        server.port = port;
        server.backlog = backlog;

        server.address.sin_family = domain;
        server.address.sin_port = htons(port);
        server.address.sin_addr.s_addr = htonl(interface);

        server.socket = socket(domain, service, protocol);

        if (0 == server.socket)
        {
            exit(1);
        }

        int bind_status = bind(server.socket, (struct sockaddr *)&server.address, sizeof(server.address));
       
        if (0 > bind_status)
        {
            exit(1);
        }

        int listen_status = listen(server.socket, backlog);
     
        if (0 > listen_status)
        {
            exit(1);
        }

        server.launch = launch;

        return server;

    }



