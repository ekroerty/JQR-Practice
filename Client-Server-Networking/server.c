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
#include "./worker.h"


#define PORT 4433
#define THREAD_COUNT 2
#define N_FAILURE -1

void data_free(void * p_data_void)
{
    fight_data_t * p_data = (fight_data_t *)p_data_void;
    free(p_data->pp_fighter[0]);
    free(p_data->pp_fighter[1]);
    free(p_data->pp_fighter);
    p_data->client_fds[0] = 0;
    p_data->client_fds[1] = 0;
    free(p_data->client_fds);
    free(p_data);
}

bool gb_run = true;

void sighandler(int signum)
{
    printf("\nCaught signal %d. Gracefully exiting...\n", signum);
    gb_run = false;
}

// this will be where I error check the packet that the client send 
bool check_packet(char * client_msg)
{
    if (client_msg)
    {
        return true;
    }
    return false;
}

fighter_t * handle_connections(void * sock_void)
{
    int client_sock = *(int *)sock_void;
    socklen_t addr_size;
    struct sockaddr_in client_addr;
    char buffer[1024];
    char from_client[1024];
    // char kill[4] = "kill";
    // char rec[9] = "Received\0";
    // char shut_msg[9] = "Shutdown\0";
    char hello_msg[23] = "Hello from the server.\0";
    bool shut = false;
    int client_kill = 0;
    fighter_t * p_fighter;

    // sends a confirmation to the client of the connection
    memset(buffer, '\0', sizeof(buffer));

    printf("[SERVER MESSAGE] ");
    snprintf(buffer, 24, "%s", hello_msg);

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
    }

    // prints client message and sends an ack

    // if client receives no ack, it will print error in sending message
    // to the command line
    if (gb_run)
    {   

        if (!check_packet(from_client))
        {
            close(client_sock);
            printf("[ERROR] Received malformed packet from CLIENT %d.\n\n", client_sock);
            printf("[DISCONNECTED] Connection from CLIENT %d closed.\n\n", client_sock);
        }

        printf("[CLIENT %d] %s", client_sock, from_client);

        p_fighter = calloc(1, sizeof(fighter_t));

        char len_str = (from_client[0]);

        int name_len = len_str - '0';
        printf("Name length: %d\n", name_len);

        char client_char;

        for (int iter = 1; iter < (name_len + 1); iter++)
        {
            client_char = from_client[iter];
            strncat(p_fighter->name, &client_char, 1);
        }

        printf("Name: %s\n", p_fighter->name);

        int start = (name_len + 1);
        char * attack_str = calloc(3, sizeof(char));
        char * dodge_str = calloc(3, sizeof(char));
        char * luck_str = calloc(3, sizeof(char));

        strncat(attack_str, &from_client[start], 2);
        strncat(dodge_str, &from_client[start+2], 2);
        strncat(luck_str, &from_client[start+4], 2);

        p_fighter->attack = (uint8_t)strtoul(attack_str, (char **)NULL, 10);
        p_fighter->dodge = (uint8_t)strtoul(dodge_str, (char **)NULL, 10);
        p_fighter->luck = (uint8_t)strtoul(luck_str, (char **)NULL, 10);

        free(attack_str);
        free(dodge_str);
        free(luck_str);

        printf("Attack: %d | Dodge: %d | Luck: %d \n", p_fighter->attack, p_fighter->dodge, p_fighter->luck);

        memset(buffer, '\0', sizeof(buffer));

        return p_fighter;
    }

    return NULL;
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
    int32_t client_sock;
    char * ip = "127.0.0.1";
    int backlog = 5;
    char buffer[1024];
    char from_client[1024];
    char win_msg[1024];
    fighter_t ** pp_fighters = calloc(2, sizeof(fighter_t *));
    int32_t * client_socks = calloc(2, sizeof(int32_t));
    fight_data_t * p_data = NULL;

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
        
        if (0 > client_sock)
        {
            continue;
        }

        printf("[CONNECTED] New connection from %d\n", client_sock);

        if (!pp_fighters[0])
        {
            pp_fighters[0] = handle_connections(&client_sock);
            client_socks[0] = client_sock;
            continue;
        }
        else if (!pp_fighters[1])
        {
            pp_fighters[1] = handle_connections(&client_sock);
            client_socks[1] = client_sock;
        }

        p_data = calloc(1, sizeof(fight_data_t));

        p_data->pp_fighter = pp_fighters;
        p_data->client_fds = client_socks;

        tpool_add_job(p_tpool, decide_winner, p_data, data_free);

        pp_fighters = calloc(2, sizeof(fighter_t *));
        client_socks = calloc(2, sizeof(int32_t));
       
    }

    // data_free(p_data);
    free(pp_fighters[0]);
    free(pp_fighters[1]);
    free(pp_fighters);
    free(client_socks);
    free(p_data);
    tpool_destroy(&p_tpool);
    return 0;
}
