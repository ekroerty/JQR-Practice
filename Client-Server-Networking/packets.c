#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>

typedef struct conn_req_t
{
    uint8_t pack_flag;
    uint8_t req_flag;
} conn_req_t;

typedef struct conn_res_t
{
    uint8_t pack_flag;
    uint8_t res_flag;
    uint64_t sess_id;
} conn_res_t;

typedef struct conn_fail_t
{
    uint8_t pack_flag;
    uint8_t fail_flag;
    uint8_t busy_flag;
} conn_fail_t;

typedef struct fight_req_t
{
    uint8_t pack_flag;
    uint8_t req_flag;
    uint64_t sess_id;
    uint8_t attk;
    uint8_t def;
    uint8_t luck;
    uint8_t name_len;
    char name[10];

} fight_req_t;

typedef struct fight_ack_t
{
    uint8_t pack_flag;
    uint8_t ack_flag;
    uint8_t result;
    uint8_t opp_attk;
    uint8_t opp_def;
    uint8_t opp_luck;
    uint8_t opp_name_len;
    char opp_name[10];

} fight_ack_t;

typedef struct fight_rej_t
{
    uint8_t pack_flag;
    uint8_t rej_flag;
    uint8_t reason_code;

} fight_rej_t;

typedef struct dis_req_t
{
    uint8_t pack_flag;
    uint8_t req_flag;
    struct in_addr worker_ip;
    uint8_t tcp_port;

} dis_req_t;


typedef struct dis_res_t
{
    uint8_t pack_flag;
    uint8_t res_flag;
    struct in_addr server_ip;
} dis_res_t;

typedef struct dis_ack_t
{
    uint8_t pack_flag;
    uint8_t ack_flag;

} dis_ack_t;

typedef struct dis_rej_t
{
    uint8_t pack_flag;
    uint8_t rej_flag;
} dis_rej_t;

typedef struct work_awake_t
{
    uint8_t pack_flag;
    uint8_t awake_flag;
} work_awake_t;

typedef struct work_data_t
{
    uint8_t pack_flag;
    uint8_t data_flag;
    uint8_t f1_attk;
    uint8_t f1_def;
    uint8_t f1_luck;
    uint8_t f2_attk;
    uint8_t f2_def;
    uint8_t f2_luck;
} work_data_t;

typedef struct work_res_t
{
    uint8_t pack_flag;
    uint8_t res_flag;
    uint8_t ret_code;
} work_res_t;

typedef struct work_fail_t
{
    uint8_t pack_flag;
    uint8_t opcode;
} work_fail_t;
