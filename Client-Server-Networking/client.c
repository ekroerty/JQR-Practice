#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <signal.h>
#include <stdbool.h>
#include <arpa/inet.h>

#define N_FAILURE -1
bool gb_var = true;

char * input_val(char *);


void sighandler(int signum)
{
    printf("\nCaught signal %d. Gracefully exiting...\n", signum);
    gb_var = false;
}

bool name_check(char * buff)
{
    if (10 <= strnlen(buff, 11))
    {
        printf("[ERROR] Name cannot contain 10 or more characters. Try again...\n");
        return false;
    }
    return true;
}

bool num_check(char * buff)
{
    int buff_int = strtol(buff, (char **)NULL, 10);
    if ((buff_int > 80) || (10 > buff_int))
    {
        printf("[ERROR] Stat must be greater than 10 and less than 80. Try again...\n");
        return false;
    }
    return true;
}

void luck_check(char * buffer, char * attack, char * dodge, char * luck_str)
{
    int atk_int = strtol(attack, (char **)NULL, 10);
    int dodge_int = strtol(dodge, (char **)NULL, 10);
    int luck_int = (100 - atk_int - dodge_int);
    if ((luck_int > 80 ) || (luck_int < 10))
    {
        printf("[ERROR] All stats must add up to 100. Try again.\n");
        memset(buffer, '\0', sizeof(buffer));
        free(luck_str);
        input_val(buffer);
    }
    else
    {
        snprintf(luck_str, sizeof(luck_str), "%d", luck_int);
    }

}
char * input_val(char * buffer)
{
    char name_buff[100];
    char atk_buff[4];
    char dodge_buff[4];
    char luck_buff[4];
    memset(name_buff, '\0', sizeof(name_buff));
    printf("Enter fighter name: ");
    fgets(name_buff, sizeof(name_buff), stdin);

    while (!name_check(name_buff))
    {
        printf("Enter fighter name: ");
        fgets(name_buff, sizeof(name_buff), stdin);
    }

    int name_len = (strnlen(name_buff, 10) - 1);

    name_buff[name_len] = '\0';

    memset(atk_buff, '\0', sizeof(atk_buff));
    printf("Enter fighter attack stat: ");
    fgets(atk_buff, sizeof(atk_buff), stdin);
    while (!num_check(atk_buff))
    {
        printf("Enter fighter attack stat: ");
        fgets(atk_buff, sizeof(atk_buff), stdin);
    }
    atk_buff[2] = '\0';

    memset(dodge_buff, '\0', sizeof(dodge_buff));
    printf("Enter fighter dodge stat: ");
    fgets(dodge_buff, sizeof(dodge_buff), stdin);
    while (!num_check(dodge_buff))
    {
        printf("Enter fighter dodge stat: ");
        fgets(dodge_buff, sizeof(dodge_buff), stdin);
    }
    dodge_buff[2] = '\0';

    memset(luck_buff, '\0', sizeof(luck_buff));

    int atk_int = strtol(atk_buff, (char **)NULL, 10);
    int dodge_int = strtol(dodge_buff, (char **)NULL, 10);
    int luck_int = (100 - atk_int - dodge_int);

    if ((luck_int > 80 ) || (luck_int < 10))
    {
        printf("All stats must add up to 100.\n");
        return NULL;
    }
    snprintf(luck_buff, sizeof(luck_buff), "%d", luck_int);

    printf("Fighter luck stat: %s\n", luck_buff);
    luck_buff[2] = '\0';


    // printf("Name: %s, name length: %s\n", name_buff, name_len);
    printf("Attack: %s | Dodge: %s | Luck: %s \n", atk_buff, dodge_buff, luck_buff);
    

    memset(buffer, '\0', sizeof(buffer));

    snprintf(buffer, (name_len + 11), "%d%s%s%s%s", 
        name_len, name_buff, atk_buff, dodge_buff, luck_buff);

    // printf("Buffer: %s\n", buffer);

    return buffer;
}

int main()
{
    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    char * p_buffer;
    char * ip = "10.30.143.25";
    int port = 4433;
    int bind_status, listen_status;
    int backlog = 5;
    char shutdown[8] = "Shutdown";
    char rec[8] = "Received";
    int sock_status, connect_status;

    sock_status = socket(AF_INET, SOCK_STREAM, 0);

    if (0 > sock_status)
    {
        return N_FAILURE;
    }

    memset(&addr, '\0', sizeof(addr));

    addr.sin_family = AF_INET; //ipv4
    addr.sin_port = htons(port); //port number
    addr.sin_addr.s_addr = inet_addr(ip); //ip address

    // connect to the server
    connect_status = connect(sock_status, (struct sockaddr*)&addr, sizeof(addr));
    if (0 > connect_status)
    {
        printf("Unable to connect to server. Exiting...\n");
        return N_FAILURE;
    }
    printf("[CONNECTED] Connected to the server.\n");

    memset(buffer, '\0', sizeof(buffer));
    recv(sock_status, buffer, sizeof(buffer), 0);
    printf("[SERVER] %s\n", buffer);

    memset(buffer, '\0', sizeof(buffer));
    p_buffer = input_val(buffer);

    while (!p_buffer)
    {
        printf("[ERROR] Data is malformed. Try again.\n");
        p_buffer = input_val(buffer);
    }

    // printf("Buffer 2: %s\n", buffer);

    send(sock_status, p_buffer, strlen(buffer), 0);

    memset(buffer, '\0', sizeof(buffer));
    recv(sock_status, buffer, sizeof(buffer), 0);

    printf("The winner is %s!\n", buffer);

    close(sock_status);
    printf("[DISCONNECTED] Disconnected from the server.\n");

    return 0;

}