/**
 * @file server_client_actions.h
 * @author Emma Roerty
 * @brief Header file for public functions that enables
 *        the user to take multiple actions.
 * @version 0.1
 * @date 2023-03-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../include/server.h"
#include <dirent.h>

void
recv_msgs (conn_data_t *, client_t *);