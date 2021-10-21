/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2018 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2015      Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ompi_config.h"

#include "ompi/mpi/c/bindings.h"
#include "ompi/runtime/params.h"
#include "ompi/errhandler/errhandler.h"
#include "ompi/runtime/ompi_spc.h"
#include "ompi/mca/pml/pml.h"
#include "ompi/mca/op/op.h"
#include "ompi/mca/coll/tuned/coll_tuned.h"
#include "ompi/datatype/ompi_datatype_inc.h"
#include <libinc_finalize.h>

#if OMPI_BUILD_MPI_PROFILING
#if OPAL_HAVE_WEAK_SYMBOLS
#pragma weak MPI_Finalize = PMPI_Finalize
#endif
#define MPI_Finalize PMPI_Finalize
#endif

#define MCA_COLL_BASE_TAG_NEWREDUCE 0xff3344f4
#define RC_APPEND_SIZE 14
static const char FUNC_NAME[] = "MPI_Finalize";


void ompi_finalize(struct ompi_communicator_t *comm){
    printf("enter fun ompi_finalize()-------comm->c_contextid:%d\n",comm->c_contextid);
    int comm_rank, comm_size, err;
    comm_rank = ompi_comm_rank(comm);
    comm_size = ompi_comm_size(comm);

    int server_per = ompi_process_info.num_local_peers;
    int server_rank_num = server_per + 1;
    int server_num = comm_size / server_rank_num;
    if (comm->INC_socket_num >= 2 && server_rank_num > 1) {
        server_rank_num /= comm->INC_socket_num;
        server_num *= comm->INC_socket_num;
    }

    if ((comm_rank % server_rank_num == 0) && (server_num > 1)){
        int inc_switch_query_size = sizeof(INC_switch_query);
        int inc_data_size = sizeof(INC_data);
        int finalize_count = inc_data_size + inc_switch_query_size;
        int rc_append_size = RC_APPEND_SIZE;
        if (comm->INC_ucx_tls){
            finalize_count += rc_append_size;
        }
        char *finalize_buffer = (char *)malloc(finalize_count);
        if(comm_rank > (comm->INC_socket_num - 1) * server_rank_num) {
            err = MCA_PML_CALL(recv(finalize_buffer, finalize_count, MPI_BYTE,((comm_rank / server_rank_num) % comm->INC_socket_num) * server_rank_num, MCA_COLL_BASE_TAG_NEWREDUCE, comm, MPI_STATUSES_IGNORE));
        } else {
            INC_data *inc_final;
            INC_switch_query *inc_switch_query;
            if (comm->INC_ucx_tls) {
                inc_final = (INC_data *) (finalize_buffer + rc_append_size);
                inc_switch_query = (INC_switch_query * )(finalize_buffer + rc_append_size + inc_data_size);
            } else {
                inc_final = (INC_data *)finalize_buffer;
                inc_switch_query = (INC_switch_query * )(finalize_buffer + inc_data_size);
            }
            ompi_finalize_mpi_head(inc_final, comm_rank);
            ompi_finalize_switch_query(inc_switch_query, comm);
            for (int i = 1; i < server_num / comm->INC_socket_num; i++) {
                int destination_node = comm_rank + comm->INC_socket_num * server_rank_num * i;
                inc_switch_query->dst_rank = htonl(destination_node);
                err = MCA_PML_CALL(send(finalize_buffer, finalize_count, MPI_BYTE, destination_node, MCA_COLL_BASE_TAG_NEWREDUCE, MCA_PML_BASE_SEND_STANDARD, comm));
            }
        }
        free(finalize_buffer);
        finalize_buffer = NULL;
    }


    if(server_rank_num > 1 && comm->newcomm != NULL){
       ompi_comm_free(&comm->newcomm);
       comm->newcomm = NULL;
    }
}

int MPI_Finalize(void)
{

    if (ompi_mpi_comm_world.comm.INC_switch_info_got) { // free switch source
        ompi_finalize(&ompi_mpi_comm_world.comm);
    }


    /* If --with-spc and ompi_mpi_spc_dump_enabled were specified, print
     * all of the final SPC values aggregated across the whole MPI run.
     * Also, free all SPC memory.
     */
    SPC_FINI();

    OPAL_CR_FINALIZE_LIBRARY();

    if (MPI_PARAM_CHECK) {
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
    }

    /* Pretty simple */

    return ompi_mpi_finalize();
}

