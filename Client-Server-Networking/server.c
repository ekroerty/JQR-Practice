#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <fcntl.h>
#include "./CSD-D-JQR-Project-6-thread-pool/module_1/include/tpool.h"
#include "./server.h"


#define PORT 4433
#define THREAD_COUNT 4
#define N_FAILURE -1

bool gb_run = true;
int client_sock_actual;

void sighandler(int signum)
{
    printf("\nCaught signal %d. Gracefully exiting...\n", signum);
    gb_run = false;
}

void handle_connections(void * sock_void)
{
    int client_sock = *(int *)sock_void;
    socklen_t addr_size;
    struct sockaddr_in client_addr;
    char buffer[1024];
    char from_client[1024];
    char kill[4] = "kill";
    char rec[8] = "Received";
    char shut_msg[8] = "Shutdown";
    char hello_msg[22] = "Hello from the server.";
    bool shut = false;


    // sends a confirmation to the client of the connection
    
    printf("[SERVER MESSAGE] ");
    if (shut)
    {
        snprintf(buffer, 9, "%s", shut_msg);
    }
    else
    {
        snprintf(buffer, 23, "%s", hello_msg);
    }
    printf("%s\n", buffer);
    send(client_sock, buffer, strlen(buffer), 0);
    
    // receives data from the client
    memset(from_client, '\0', sizeof(from_client));
    while (-1 == recv(client_sock, from_client, sizeof(from_client), MSG_DONTWAIT))
    {
        if (!gb_run)
        {
            break;
        }
        continue;
    }

    // prints client message and sends an ack
    if (gb_run && !shut)
    {
        printf("[CLIENT %d] %s", client_sock, from_client);

        if (0 == strncmp(from_client, kill, 4))
        {
            printf("Exiting...\n");
            gb_run = false;
        }

        snprintf(buffer, 9, "%s", rec);
        send(client_sock, buffer, strlen(buffer), 0);   
    }

    //closes client connection immediately
    close(client_sock);
    printf("[DISCONNECTED] Connection from CLIENT %d closed.\n\n", client_sock);
}

server_t set_params(int32_t domain, int32_t protocol, int32_t service, 
                    uint32_t interface, uint32_t port, uint32_t backlog, 
                    struct sockaddr_in addr, char * ip)
{
    server_t server;
    int enable = 1;
    
    server.domain = domain;
    server.protocol = protocol;
    server.service = service;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;
    
    memset(&server.addr, '\0', sizeof(server.addr));
    server.addr.sin_family = domain; //ipv4
    server.addr.sin_port = htons(port); //port number
    server.addr.sin_addr.s_addr = inet_addr(ip); //ip address

    int server_sock = socket(domain, service, protocol);
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    if (0 > server_sock)
    {
        exit(1);
    }

    server.socket = server_sock;
    int bind_status = bind(server.socket, (struct sockaddr *)&server.addr, sizeof(server.addr));

    if (0 > bind_status)
    {
        exit(1);
    }

    int listen_status = listen(server_sock, backlog);
    
    if (0 > listen_status)
    {
        exit(1);
    }

    return server;
}

int main ()
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    int client_sock;
    char * ip = "127.0.0.1";
    int backlog = 5;
    char shutdown[8] = "shutdown";

    server_t server = set_params
    (AF_INET, 0 , SOCK_STREAM, INADDR_ANY, PORT, backlog, server_addr, ip);
    
    printf("Press CTRL+C to shutdown server.\n");
    printf("[LISTENING] Port %d...\n", PORT);
    fcntl(server.socket, F_SETFL, O_NONBLOCK);

    tpool_t * p_tpool = tpool_create(THREAD_COUNT);
    signal(SIGINT, sighandler);

    while (gb_run)
    {
        addr_size = sizeof(&client_addr);
        client_sock = accept(server.socket, (struct sockaddr *)&client_addr, &addr_size);

        if (-1 == client_sock)
        {
            continue;
        }
        else
        {
            client_sock_actual = client_sock;
        }

        if (!gb_run)
        {
            if (-1 != client_sock)
            {
                send(client_sock, shutdown, strlen(shutdown), 0);
                close(client_sock);
            }

            break;
        }
        
        if (0 > client_sock)
        {
            return N_FAILURE;
        }

        printf("[CONNECTED] New connection from %d\n", client_sock);

        //after connection is accepted, then add the job to the thread pool with client socket
        tpool_add_job(p_tpool, handle_connections, &client_sock_actual, NULL);

    }

    tpool_destroy(&p_tpool);
    return 0;
}
