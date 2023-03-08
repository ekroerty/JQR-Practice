#include "../include/server.h"
#include "../include/server_client_actions.h"
#include <string.h>
#include <byteswap.h>

#define BUFF_SIZE 1024
#define CREATE 0x01
#define LOGIN 0x02


htable_node_t *
verify_client (conn_data_t *, void *, int);

void print_list (list_t * p_cll)
{
    list_node_t *node = p_cll->p_head;
    // printf("list: ");
    for (uint32_t i = 0; i < p_cll->size; i++)
    {
        printf("%s: ", (char *)((htable_node_t *)node->p_data)->p_key);
        printf("%s, ", (char *)(((client_t *)((htable_node_t *)node->p_data)->p_value))->p_password);
        node = node->p_next;
    }
    printf("\n");
}

void print_htable_strs(htable_t * p_htable)
{
    printf("\nHash Table: \n");

    for (uint32_t iter = 0; iter < p_htable->capacity; iter ++)
    {
        printf("%d: ", iter);

        list_t * p_list = p_htable->p_hash_vals[iter];
        if ((p_list) && (0 < p_list->size))
        {
            print_list(p_list);
        }

        else
        {
            printf ("Empty.\n");
        }
    }
}

uint32_t hash_f(void * p_data, uint32_t size)

{
    uint32_t hash = 0;
    char * p_str = (char *)p_data;
    for (uint32_t i = 0; i < strnlen(p_str, 100); i++)
    {
        hash += p_str[i];
    }
    return hash % size;
}


int comp_f (const void * item1, const void * item2)
{
    char * str1 = (char *)((htable_node_t *)item1)->p_key;
    char * str2 = (char *)item2;
    int comp = strncmp(str1, str2, UNAME_MAX);
    return (comp);
}

void
client_free(void * p_node)
{
    client_t * p_client = ((client_t *)p_node);
    FREE(p_client);
}

void 
send_fail_pack (conn_data_t * p_conn, int opcode)
{
    failure_t fail_pack;
    fail_pack.pack_flag = FAILURE;
    fail_pack.opcode = opcode;
    send(p_conn->fd, &fail_pack, sizeof(failure_t), 0);
    close(p_conn->fd);
    return;
}

void
create_account (conn_data_t * p_conn, init_pack_t create_pack)
{

    if (!p_conn)
    {
        send_fail_pack(p_conn, SERVER_ERROR);
        return;
    }

    printf("Creating new account!\n");

    objects_t * p_objects = p_conn->p_objects;

    if (!p_objects)
    {
        send_fail_pack(p_conn, SERVER_ERROR);
        return;
    }

    pthread_mutex_lock(&p_objects->p_signals->client_mutex);
    htable_t * p_htable = p_objects->p_htable;
    pthread_mutex_unlock(&p_objects->p_signals->client_mutex);

    if (!p_htable)
    {
        printf("Creating database!\n");
        pthread_mutex_lock(&p_objects->p_signals->client_mutex);
        p_objects->p_htable = htable_create(hash_f, comp_f);
        pthread_mutex_unlock(&p_objects->p_signals->client_mutex);
    }

    client_t * p_client = calloc(1, sizeof(client_t));

    if (!p_client)
    {
        send_fail_pack(p_conn, SERVER_ERROR);
        return;
    }

    p_client->fd = p_conn->fd;
    p_client->p_addr = p_conn->p_addr;

    pthread_mutex_lock(&p_conn->p_objects->p_signals->sess_mutex);
    p_client->session_id = ++p_conn->p_objects->session_id;
    pthread_mutex_unlock(&p_conn->p_objects->p_signals->sess_mutex);

    strncpy(p_client->p_username, create_pack.uname, create_pack.uname_len+1);
    strncpy(p_client->p_password, create_pack.password, create_pack.pass_len+1);

    //check to see if the uname already exists

    pthread_mutex_lock(&p_objects->p_signals->client_mutex);
    bool insert = htable_insert(&p_objects->p_htable, 
                   p_client->p_username, 
                   p_client,
                   client_free);
   
    if (!insert)
    {
        pthread_mutex_unlock(&p_objects->p_signals->client_mutex);
        send_fail_pack(p_conn, SERVER_ERROR);
        FREE(p_client);
        return;
    }
    // p_objects->p_htable = p_htable_rehash;
    print_htable_strs(p_objects->p_htable);
    pthread_mutex_unlock(&p_objects->p_signals->client_mutex);
    printf("Added new user to the database!\n");

    init_ack_t ack_pack;
    ack_pack.pack_flag = INIT;
    ack_pack.res_flag = ACKNOWLEDGE;
    ack_pack.session_id = p_client->session_id;

    send(p_client->fd, &ack_pack, sizeof(init_ack_t), 0);
    close(p_client->fd);
    return;
}

void
handle_login (conn_data_t * p_conn, init_pack_t login_pack)
{
    if (!p_conn)
    {
        send_fail_pack(p_conn, SERVER_ERROR);
        return;
    }

    objects_t * p_objects = p_conn->p_objects;

    if (!p_objects)
    {
        send_fail_pack(p_conn, SERVER_ERROR);
        return;
    }


    pthread_mutex_lock(&p_objects->p_signals->client_mutex);
    htable_t * p_htable = p_objects->p_htable;
    pthread_mutex_unlock(&p_objects->p_signals->client_mutex);

    if (!p_htable)
    {
        printf("Invalid client given\n");
        send_fail_pack(p_conn, INVALID_UNAME);
        return;
    }

    htable_node_t * p_node = verify_client(p_conn, &login_pack, LOGIN);
    
    if (!p_node)
    {
       return;
    }
    
    client_t * p_client = p_node->p_value;

    if (!p_client)
    {
        return;
    }

    pthread_mutex_lock(&p_objects->p_signals->client_mutex);
    uint8_t num_users = p_objects->p_htable->curr_size;
    pthread_mutex_unlock(&p_objects->p_signals->client_mutex);

    if (0 != strncmp(login_pack.password, p_client->p_password, PASS_MAX))
    {
        send_fail_pack(p_conn, INVALID_LOGIN);
        return;
    }

    login_ack_t login_ack;
    login_ack.pack_flag = LOGIN;
    login_ack.res_flag = ACKNOWLEDGE;
    login_ack.num_users = num_users;

    p_client->fd = p_conn->fd;
    print_htable_strs(p_objects->p_htable);
    send(p_client->fd, &login_ack, sizeof(login_ack), 0);

    recv_msgs(p_conn, p_client);
}

htable_node_t *
verify_client (conn_data_t * p_conn, void * any_pack, int opt)
{
    htable_node_t * p_node = NULL;
    update_pack_t * p_update = NULL;

    if ((CREATE == opt) || (LOGIN == opt) || (DELETE == opt) || (UPDATE == opt))
    {
        p_update = (update_pack_t *)any_pack;
    }

    else
    {
        return NULL;
    }

    pthread_mutex_lock(&p_conn->p_objects->p_signals->client_mutex);
    p_node = htable_find(p_conn->p_objects->p_htable, 
                         p_update->uname);
    pthread_mutex_unlock(&p_conn->p_objects->p_signals->client_mutex);

    if (!p_node)
    {
        printf("Invalid client given, %s not found\n", p_update->uname);
        send_fail_pack(p_conn, INVALID_UNAME);
        return NULL;
    }

    client_t * p_client = ((client_t *)p_node->p_value);

    if (0 != strncmp(p_update->password, p_client->p_password, PASS_MAX))
    {
        send_fail_pack(p_conn, INVALID_LOGIN);
        return NULL;
    }

    printf("Login verified!\n");
    return p_node;
}

void
remove_client (conn_data_t * p_conn, init_pack_t pack)
{
    if (!p_conn)
    {
        send_fail_pack(p_conn, SERVER_ERROR);
        return;
    }

    htable_node_t * p_node = verify_client(p_conn, &pack, DELETE);

    if (!p_node)
    {
        return;
    }

    client_t * p_client = p_node->p_value;

    if (!p_client)
    {
        return;
    }

    // htable_node_t * p_node = NULL;
    p_node = htable_remove(p_conn->p_objects->p_htable, 
                        p_client->p_username);
    
    if (p_node->free_f)
    {
        p_node->free_f(p_node->p_value);
    }

    FREE(p_node);
    print_htable_strs(p_conn->p_objects->p_htable);
    pthread_mutex_unlock(&p_conn->p_objects->p_signals->client_mutex);
}

void
update_client (conn_data_t * p_conn, update_pack_t pack)
{
    if (!p_conn)
    {
        send_fail_pack(p_conn, SERVER_ERROR);
        return;
    }

    printf("Username: %s | Old password: %s | New password: %s\n", 
        pack.uname, pack.password, pack.new_password);

    htable_node_t * p_node = verify_client(p_conn, &pack, UPDATE);

    if (!p_node)
    {
        return;
    }

    client_t * p_client = p_node->p_value;
    strncpy(p_client->p_password, pack.new_password, pack.new_pass_len+1);

    // if (!update)
    // {
    //     send_fail_pack(p_conn, SERVER_ERROR);
    //     return;
    // }

    update_ack_t update_ack;
    update_ack.pack_flag = UPDATE;
    update_ack.res_flag = ACKNOWLEDGE;
    send(p_conn->fd, &update_ack, sizeof(update_ack_t), 0);
    close(p_conn->fd);
    return;

}



void
handle_clients (conn_data_t * p_conn)
{
    char buff[BUFF_SIZE];

    memset(&buff, '\0', BUFF_SIZE);

    if (!p_conn)
    {
        return;
    }

    objects_t * p_objects = p_conn->p_objects;

    if (!p_objects)
    {
        return;
    }

    while (-1 == recv(p_conn->fd, buff, BUFF_SIZE, MSG_DONTWAIT))
    {
        if (!(*p_objects->pb_run))
        {
            return;
        }
    }

    // printf("Buffer: %s\n", buff);
    switch (buff[0])
    {
        case CREATE:
        {
            //do something
            init_pack_t create_pack;
            memcpy(&create_pack, buff, sizeof(init_pack_t));
            printf("Username: %s | Password: %s\n", create_pack.uname, create_pack.password);
            create_account(p_conn, create_pack);
            break;
        }
        case LOGIN:
        {
            init_pack_t login_pack;
            memcpy(&login_pack, buff, sizeof(init_pack_t));
            printf("Username: %s | Password: %s\n", login_pack.uname, login_pack.password);
            handle_login(p_conn, login_pack);
            break;
        }
        case DELETE:
        {
            init_pack_t del_pack;
            memcpy(&del_pack, buff, sizeof(init_pack_t));
            printf("Username: %s | Password: %s\n", del_pack.uname, del_pack.password);
            remove_client(p_conn, del_pack);
            break;
        }
        case UPDATE:
        {
            // printf("Update case\n");
            // printf("Buffer: %s\n", buff);
            update_pack_t update_pack;
            memcpy(&update_pack, buff, sizeof(update_pack_t));
            // printf("Pack flag: %d\n", update_pack.pack_flag);
            printf("Username: %s | Password: %s\n", update_pack.uname, update_pack.password);
            update_client(p_conn, update_pack);
            break;
        }
        default:
            break;
    }
    return;

}