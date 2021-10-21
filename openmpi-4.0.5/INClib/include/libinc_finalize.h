#ifndef __LIBINC_FINALIZE_H__
#define __LIBINC_FINALIZE_H__
void ompi_finalize_mpi_head(INC_data * inc_final, int comm_rank) ;

void ompi_finalize_switch_query(INC_switch_query *inc_switch_query, struct ompi_communicator_t *comm) ;
#endif
