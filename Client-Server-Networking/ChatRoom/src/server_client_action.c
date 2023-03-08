/**
 * @file server_client_action.c
 * @author Emma Roerty
 * @brief Source code for actions the client can take.
 * @version 1.0
 * @date 2023-03-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../include/server_client_actions.h"
#define FILE_OPTIONS 0x09
#define REQUEST 0x01

void
send_dir (client_t * p_client) {
    DIR *d;
    struct dirent *dir;
    d = opendir("./server_files");
    dir_files_t file;
    eof_t eof;
    eof.pack_flag = EOF;
    memset(&file, '\0', sizeof(dir_files_t));

    if (d) 
    {
        while ((dir = readdir(d)) != NULL) 
        {
            if ((0 == strncmp(dir->d_name, ".", 1))
              ||(0 == strncmp(dir->d_name, "..", 2)))
            {
                continue;
            }

            file.pack_flag = FILE_OPTIONS;
            snprintf(file.file_name, 256, "%s", dir->d_name);
            file.name_len = strnlen(dir->d_name, UINT8_MAX);
            // printf("File name: %s | Length %d\n", file.file_name, file.name_len);
            send(p_client->fd, &file, sizeof(dir_files_t), 0);
            memset(&file, '\0', sizeof(dir_files_t));
        }

        send(p_client->fd, &eof, sizeof(eof_t), 0);
        closedir(d);
    }
    return;
}

void
handle_file (conn_data_t  * p_conn,
             file_share_t * p_file_pack,
             client_t     * p_client)
{
    if (!p_conn || !p_file_pack)
    {
        return;
    }

    char buffer[BUFF_SIZE + 1] = {0};
    uint32_t recv_out;
    uint32_t running_size = 0;
    FILE * p_file = NULL;
    int file_len = p_file_pack->file_name_len + 16;
    char filename[file_len];
    memset(&filename, '\0', file_len);
    snprintf(filename, file_len, 
            "./server_files/%s", p_file_pack->file_name);

    printf("Filename: %s\n", filename);
    // const char * p_filename = filename;
    p_file = fopen(filename, "wb");

    if (!p_file)
    {
        printf("Unable to open file.\n");
        return;
    }

    while (*(p_conn->p_objects->pb_run))
    {
        recv_out = recv(p_conn->fd, &buffer, BUFF_SIZE, 0);
        running_size += recv_out;

        if (recv_out <= 0)
        {
            printf("Nothing to receive.\n");
            break;
            // return;
        }

        fprintf(p_file, "%s", buffer);

        if (running_size == p_file_pack->file_size)
        {
            printf("File successfully uploaded!\n");
            printf("Number of bytes: %d\n", running_size);
            break;
        }

        memset(buffer, '\0', BUFF_SIZE);
    }

    fclose(p_file);
    recv_msgs(p_conn, p_client);
}

void
handle_download(conn_data_t  * p_conn,
                download_t   * p_download,
                client_t     * p_client)
{
    if (!p_conn || !p_download)
    {
        return;
    }

    char buffer[BUFF_SIZE + 1] = {0};
    int file_len = p_download->file_name_len + 16;
    char filename[file_len];
    memset(&filename, '\0', file_len);
    snprintf(filename, file_len, "./server_files/%s", p_download->file_name);

    printf("Filename: %s\n", filename);

    FILE * p_file = fopen(filename, "rb");

    if (!p_file)
    {
        printf("Unable to open file. Exiting...\n");
        return;
    }

    while (NULL != fgets(buffer, BUFF_SIZE, p_file))
    {
        if (-1 == send(p_client->fd, buffer, sizeof(buffer) - 1, 0))
        {
            recv_msgs(p_conn, p_client);
        }

        memset(buffer, '\0', BUFF_SIZE + 1);
    }

    printf("%s downloaded by %s!\n", filename, p_client->p_username);
    fclose(p_file);
    close(p_client->fd);

}

void
broadcast_msg (conn_data_t * p_conn, client_t * p_client, msg_pack_t * p_msg)
{
    if (!p_conn || !p_client)
    {
        return;
    }

    objects_t * p_objects = p_conn->p_objects;

    for (uint32_t iter = 0; iter < p_objects->p_htable->capacity; iter++)
    {
        for (uint32_t idx = 0; idx < p_objects->p_htable->p_hash_vals[iter]->size; idx++)
        {
            client_t * p_client = (client_t *)list_index(p_objects->p_htable->p_hash_vals[iter], idx);
            send(p_client->fd, p_msg, sizeof(msg_pack_t), 0);
        }
    }
}


void
recv_msgs (conn_data_t * p_conn, client_t * p_client)
{
    char buff[BUFF_SIZE];

    while (-1 == recv(p_client->fd, &buff, BUFF_SIZE, MSG_DONTWAIT))
    {
        if (!(*p_conn->p_objects->pb_run))
        {
            return;
        }
    }

    switch (buff[0])
    {
        case DISCONNECT:
        {
            close(p_client->fd);
            return;
        }
        case MSG:
        {
            msg_pack_t * p_msg = (msg_pack_t *)buff;
            printf("Message from %s: %s\n", p_client->p_username, p_msg->msg);
            // broadcast_msg(p_conn, p_client, p_msg);
            recv_msgs(p_conn, p_client);
            break;
        }
        case FILE_UPLOAD:
        {
            printf("Preparing to receive a file...\n");
            // file_share_t * p_file_pack = (file_share_t *)buff;
            file_share_t p_file_pack;
            memcpy(&p_file_pack, buff, sizeof(file_share_t));
            handle_file(p_conn, &p_file_pack, p_client);
            break;
        }
        case FILE_DOWNLOAD:
        {

            if (buff[1] == REQUEST)
            {
                send_dir(p_client);
                recv_msgs(p_conn, p_client);
                return;
            }

            download_t * p_download = (download_t *)buff;
            handle_download(p_conn, p_download, p_client);
            break;
        }
        default:
            send_fail_pack(p_conn, SERVER_ERROR);
            return;
    }

}

