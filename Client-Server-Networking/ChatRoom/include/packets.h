#ifndef _PACKETS_H
#define _PACKETS_H

#include "./server.h"

#define UNAME_MAX 10
#define PASS_MAX 25

#pragma pack(1)

typedef struct init_pack_t
{
    uint8_t  pack_flag;
    uint8_t  uname_len;
    char     uname[UNAME_MAX];
    uint8_t  pass_len;
    char     password[PASS_MAX];

} init_pack_t;

typedef struct update_pack_t
{
    uint8_t  pack_flag;
    uint8_t  uname_len;
    char     uname[UNAME_MAX];
    uint8_t  pass_len;
    char     password[PASS_MAX];
    uint8_t  new_pass_len;
    char     new_password[PASS_MAX];

} update_pack_t;


typedef struct init_ack_t
{
    uint8_t  pack_flag;
    uint8_t  res_flag;
    uint64_t session_id;

} init_ack_t;

typedef struct update_ack_t
{
    uint8_t pack_flag;
    uint8_t res_flag;
} update_ack_t;

typedef struct login_ack_t
{
    uint8_t  pack_flag;
    uint8_t  res_flag;
    uint8_t  num_users; // only allows for 255 users
} login_ack_t;

typedef struct msg_pack_t
{
    uint8_t pack_flag;
    uint8_t msg_len;
    char    msg[255];
} msg_pack_t;

typedef struct failure_t
{
    uint8_t pack_flag;
    uint8_t opcode;
} failure_t;

typedef struct disconn_t
{
    uint8_t pack_flag;
} disconn_t;

typedef struct file_share_t
{
    uint8_t      pack_flag;
    uint32_t     file_size;
    uint8_t      file_name_len;
    char         file_name[255];

} file_share_t;

typedef struct download_t
{
    uint8_t      pack_flag;
    uint8_t      file_name_len;
    char         file_name[256];

} download_t;

typedef struct dir_files_t
{
    uint8_t pack_flag;
    uint8_t name_len;
    char    file_name[256];
} dir_files_t;

typedef struct eof_t
{
    uint8_t pack_flag;
} eof_t;

typedef struct reject_t
{
    uint8_t pack_flag;
    uint8_t type_flag;
    uint8_t opcode;
} reject_t;

#pragma pack()

#endif