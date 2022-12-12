#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#define N_FAILURE -1

int main()
{

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    char * ip = "127.0.0.1";
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

    memset(buffer, '\0', 1024);
    recv(sock_status, buffer, sizeof(buffer), 0);
    printf("[SERVER] %s\n", buffer);
    if (0 == strncmp(shutdown, buffer, 8))
    {
        printf("[DISCONNECTED] Disconnected from the server.\n");
        close(sock_status);
        return 0;
    }

    memset(buffer, '\0', 1024);
    printf("[YOU] ");
    fgets(buffer, 1024, stdin);
    send(sock_status, buffer, strlen(buffer), 0);

    recv(sock_status, buffer, sizeof(buffer), 0);
    if (0 != strncmp(rec, buffer, 8))
    {
        printf("[ERROR] Failed to send data to the server.\n");
    }

    close(sock_status);
    printf("[DISCONNECTED] Disconnected from the server.\n");

    return 0;

}