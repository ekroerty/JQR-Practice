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
#include "./CSD-D-JQR-Project-6-thread-pool/module_1/include/free.h"
#include "./server.h"
#include "./worker.h"

#define MYFILE "log_file.txt"
#define PORT 4433
#define THREAD_COUNT 2
#define N_FAILURE -1
bool gb_run = true;

void data_free(void * p_data_void)
{
    fight_data_t * p_data = (fight_data_t *)p_data_void;
    FREE(p_data->pp_fighter[0]);
    FREE(p_data->pp_fighter[1]);
    FREE(p_data->pp_fighter);
    p_data->client_fds[0] = 0;
    p_data->client_fds[1] = 0;
    FREE(p_data->client_fds);
    // FREE(p_data->p_file_struct);
}


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
    char hello_msg[23] = "Hello from the server.\0";
    bool shut = false;
    int client_kill = 0;
    fighter_t * p_fighter = NULL;

    // sends a confirmation to the client of the connection
    memset(buffer, '\0', sizeof(buffer));

    printf("[SERVER MESSAGE] ");
    snprintf(buffer, 24, "%s", hello_msg);

    printf("%s\n", buffer);
    send(client_sock, buffer, strlen(buffer), 0);

    printf("[WAITING] Waiting for fighter information...\n\n");
    
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
        // printf("Blank from client: %s\n", from_client);
        if (!check_packet(from_client) || !*from_client)
        {
            close(client_sock);
            printf("[ERROR] Received malformed packet from CLIENT %d.\n", client_sock);
            printf("[DISCONNECTED] Connection from CLIENT %d closed.\n\n", client_sock);
            return NULL;
        }


        p_fighter = calloc(1, sizeof(fighter_t));

        char len_str = (from_client[0]);

        int name_len = len_str - '0';
        // printf("Name length: %d\n", name_len);

        char client_char;

        for (int iter = 1; iter < (name_len + 1); iter++)
        {
            client_char = from_client[iter];
            strncat(p_fighter->name, &client_char, 1);
        }

        // printf("Name: %s\n", p_fighter->name);

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

        printf("[CLIENT %d] Name: %s | Attack: %d | Dodge: %d | Luck: %d \n", 
        client_sock, p_fighter->name, p_fighter->attack, p_fighter->dodge, p_fighter->luck);
        // printf("Attack: %d | Dodge: %d | Luck: %d \n", p_fighter->attack, p_fighter->dodge, p_fighter->luck);

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
    char * ip = "10.30.143.25";
    int backlog = 5;
    char buffer[1024];
    char outbuffer[1024];
    char timestamp[20];
    char from_client[1024];
    char win_msg[1024];
    fighter_t ** pp_fighters = calloc(2, sizeof(fighter_t *));
    int32_t * client_socks = calloc(2, sizeof(int32_t));
    fight_data_t * p_data = calloc(1, sizeof(fight_data_t));
    FILE * p_file;
    p_file = fopen(MYFILE, "w");
    time_t rawtime = time(NULL);
    struct tm * p_tm = localtime(&rawtime);

    file_t * p_file_type = calloc(1, sizeof(file_t));
    p_file_type->p_tm = p_tm;
    p_file_type->file = p_file;
    p_file_type->ip = ip;
    p_file_type->port = PORT;

    server_t server = set_params
    (AF_INET, 0 , SOCK_STREAM, INADDR_ANY, PORT, backlog, server_addr, ip);
    
    printf("Press CTRL+C to shutdown server.\n");
    printf("[LISTENING] Port %d...\n", PORT);
    fcntl(server.socket, F_SETFL, O_NONBLOCK);

    tpool_t * p_tpool = tpool_create(THREAD_COUNT);
    signal(SIGINT, sighandler);

    while (gb_run)
    {
        rawtime = time(NULL);
        addr_size = sizeof(&client_addr);
        client_sock = accept(server.socket, (struct sockaddr *)&client_addr, &addr_size);
        p_file_type->type = "Connection";

        if (0 > client_sock)
        {
            continue;
        }
        p_file_type->status = "Success";
        printf("[CONNECTED] New connection from %d\n", client_sock);

        // snprintf(outbuffer, 100, "%s %s %s:%d %s", timestamp, type, ip, PORT, status);
        p_tm = localtime(&rawtime);
        snprintf(timestamp, 20, "%d%d%d %02d:%02d:%02d", 
        (1900 + p_tm->tm_year), (p_tm->tm_mon + 1), p_tm->tm_mday, p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);

        // printf("Current time: %s\n", timestamp);
        snprintf(outbuffer, sizeof(outbuffer), "%s %s %s:%d %s\n", timestamp,
        p_file_type->type, p_file_type->ip, p_file_type->port, p_file_type->status);

        fwrite(outbuffer, sizeof(char), strlen(outbuffer), p_file);

        if (!pp_fighters[0])
        {
            pp_fighters[0] = handle_connections(&client_sock);
            client_socks[0] = client_sock;
            printf("[WAITING] Waiting for another fighter...\n\n");
            continue;
        }
        else if (!pp_fighters[1])
        {
            pp_fighters[1] = handle_connections(&client_sock);
            if (!pp_fighters[1])
            {
                continue;
            }
            client_socks[1] = client_sock;
        }

        // THIS WAS HERE FOR A REASON, I CAN'T REMEMBER WHY
        // p_data = calloc(1, sizeof(fight_data_t));

        p_data->pp_fighter = pp_fighters;
        p_data->client_fds = client_socks;
        p_data->p_file_struct = p_file_type;

        tpool_add_job(p_tpool, decide_winner, p_data, data_free);

        pp_fighters = calloc(2, sizeof(fighter_t *));
        client_socks = calloc(2, sizeof(int32_t));
        memset(outbuffer, '\0', sizeof(outbuffer));
       
    }

    fclose(p_file_type->file);

    if (pp_fighters[0])
    {
        FREE(pp_fighters[0]);
    }
    if (pp_fighters[1])
    {
        FREE(pp_fighters[1]);
    }

    FREE(p_file_type);
    FREE(pp_fighters);
    FREE(client_socks);
    FREE(p_data);
    tpool_destroy(&p_tpool);
    return 0;
}
