#include "./server.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void launch (server_t * p_server)
{
    char buffer[1024];
    char * hello = "Hello, World!";
    int address_length = sizeof(p_server->address);
    int new_socket;
    while (1)
    {
        new_socket = accept(p_server->socket, (struct sockaddr *)&p_server->address, (socklen_t *)&address_length);
        read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        write(new_socket, hello, strlen(hello));
        close(new_socket);
    }
}


int main()
{
    server_t p_server = server_create(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 4433, 10, launch);
    p_server.launch(&p_server);
}