

#ifndef OMPI_DATATYPE_INC_H_HAS_BEEN_INCLUDED
#define OMPI_DATATYPE_INC_H_HAS_BEEN_INCLUDED

#include <stdint.h>


#pragma pack(1)
typedef struct INC_DATA {
    uint32_t tag_low; 
    uint32_t src_rank;
    uint8_t data_flag;
    uint8_t comm_id;
    short rsv;
    uint8_t op_code; 
    uint8_t oper_and_type;
    uint8_t req_id; 
    uint8_t req_pkt_num;
    short pkt_para_num;
    uint32_t tag_high; 
} INC_data;
#pragma pack()

#pragma pack(1)
typedef struct INC_SWITCH_QUERY {
    short sup_mpi_type; 
    uint8_t fail_cause; 
    uint8_t master_num;
    uint8_t query_notify_hop; 
    uint8_t sup_comm_type; 
    short sup_allreduce_op; 
    short sup_data_type;
    short sup_max_data_size;
    short global_group_size; 
    short local_group_size;
    uint32_t true_comm_id;
    uint32_t ava_grp_num;
    uint32_t dst_rank; 
    uint32_t tor1_ip;
    uint32_t spine_ip;
    uint32_t tor2_ip;
    uint64_t job_id;
    uint32_t c_id;
    uint32_t world_rank;
} INC_switch_query;
#pragma pack()

#endif  /* OMPI_DATATYPE_INC_H_HAS_BEEN_INCLUDED */
