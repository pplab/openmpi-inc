#ifndef __LIBINC_COLL_H__
#define __LIBINC_COLL_H__
int ompi_inc_gen_commid(struct ompi_communicator_t * comm);

void ompi_init_mpi_head(INC_data* inc_data, int comm_rank);

void ompi_init_switch_query(int server_num, INC_switch_query *inc_switch_query, struct ompi_communicator_t *comm) ;

void ompi_switch_feature_query(struct ompi_communicator_t *comm, int comm_rank, int comm_size, int server_rank_num, int server_num) ;

void ompi_inc_control_surface(struct ompi_communicator_t *comm, int comm_rank, int comm_size, int server_rank_num, int server_num, int socket_num) ;

void ompi_host2network_trans(const void * sbuf , struct ompi_datatype_t *dtype , int count , struct ompi_communicator_t *comm);

void ompi_network2host_trans(void * rbuf , struct ompi_datatype_t *dtype , int count , struct ompi_communicator_t *comm);

void ompi_init_datasurface_mpi_head(INC_data* inc_data, struct ompi_communicator_t *comm, struct ompi_op_t * op, struct ompi_datatype_t *dtype, uint8_t round_count, int count, int comm_rank) ;

void ompi_inc_data_surface(void * sbuf, void * rbuf, int count, struct ompi_datatype_t *dtype, struct ompi_op_t * op, struct ompi_communicator_t *comm, int comm_rank, int server_rank_num) ;
#endif
