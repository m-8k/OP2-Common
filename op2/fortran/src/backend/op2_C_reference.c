/*
 * This file implements the reference (sequential) version of op_par_loop calls
 * to be used in Fortran
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../include/op2_C_reference.h"
#include "../../include/op2_reference_macros.h"
#include "../../include/op2_for_C_wrappers.h"

#include <op_lib_core.h>

#ifdef NO_MPI

#else

#include <op_lib_mpi.h>
#endif

// only for debugging!!!


int op2_stride = 1;
#define OP2_STRIDE(arr, idx) arr[idx]

char blank_args[512]; // scratch space to use for blank args

inline void op_arg_set(int n, op_arg arg, char **p_arg, int halo){
  if (arg.argtype == OP_ARG_NULL)
  { //for null arguments, p_arg is not used in the user kernel
    *p_arg = NULL;
    return;
  }

  *p_arg = arg.data;

  if (arg.argtype==OP_ARG_GBL) {
    if (halo && (arg.acc != OP_READ)) *p_arg = blank_args;
  } else if ( arg.argtype == OP_ARG_DAT ) {
    if (arg.map==NULL)         // identity mapping
      *p_arg += arg.size*n;
    else                       // standard pointers (decremented for FORTRAN)
                               // undone decrementing because everything now
                               // runs in OP2
      *p_arg += arg.size*(arg.map->map[arg.idx+n*arg.map->dim]);
  }
}

void op_arg_copy_in(int n, op_arg arg, char **p_arg) {
  for (int i = 0; i < -1*arg.idx; ++i)
    p_arg[i] = arg.data + arg.map->map[i+n*arg.map->dim]*arg.size;
}

void op_args_check(op_set set, int nargs, op_arg *args,
                                      int *ninds, const char *name) {
  for (int n=0; n<nargs; n++)
    op_arg_check(set,n,args[n],ninds,name);
}


#define CHARP_LIST(N) COMMA_LIST(N,CHARP)
#define CHARP(x) char*

#define CHARP_LIST_2(N) SEMI_LIST(N,CHARP2)
#define CHARP2(x) char* ptr##x

#define ARG_SET_LIST(N) SEMI_LIST(N,ARGSET)
#define ARGSET(x) if (args[x-1].idx < -1) op_arg_copy_in (n, args[x-1], (char **) p_a[x-1]); else {op_arg_set(n,args[x-1],&p_a[x-1],halo);}

#define PTR_LIST(N) COMMA_LIST(N,PTRL)
#define PTRL(x) p_a[x-1]

#define ZERO_LIST(N) COMMA_LIST(N,ZERO)
#define ZERO(x) 0

#define ARG_ARR_LIST(N) COMMA_LIST(N,ARG_ARR)
#define ARG_ARR(x) *arg##x

#define ARG_LIST_POINTERS(N) COMMA_LIST(N,ARG_POINTERS)
#define ARG_POINTERS(x) op_arg * arg##x

#define ALLOC_POINTER_LIST(N) SEMI_LIST(N,ALLOC_POINTER)
#define ALLOC_POINTER(x) if (arg##x->idx < -1) { p_a[x-1] = (char *) malloc (-1*args[x-1].idx*arg##x->dim);}

#define FREE_LIST(N) SEMI_LIST(N,FREE)
#define FREE(x) if (arg##x->idx < -1) {free (p_a[x-1]);}

#define REDUCE_LIST(N) SEMI_LIST(N,REDUCE)
#define REDUCE(x) if (arg##x->argtype == OP_ARG_GBL ) { if ( arg##x->acc == OP_INC || arg##x->acc == OP_MAX || arg##x->acc == OP_MIN ) { {if (strncmp (arg##x->type, "double", 6) == 0) {op_mpi_reduce_double(arg##x,(double *)p_a[x-1]);} else if (strncmp (arg##x->type, "float", 5) == 0) op_mpi_reduce_float(arg##x,(float *)p_a[x-1]); else if ( strncmp (arg##x->type, "int", 3) == 0 ){op_mpi_reduce_int(arg##x,(int *)p_a[x-1]);} else if ( strncmp (arg##x->type, "bool", 4) == 0 ) op_mpi_reduce_bool(arg##x,(bool *)p_a[x-1]); else { printf ("OP2 error: unrecognised type for reduction, type %s\n",arg##x->type); exit (0);}}}}

//      n_upper = op_mpi_halo_exchanges_seq (set, N, args);               \
//      op_mpi_wait_all_seq (N,args);                                     \


#define OP_LOOP(N) \
  void op_par_loop_##N(void (*kernel)(CHARP_LIST(N)), op_set_core * set, ARG_LIST_POINTERS(N)) { \
    char * p_a[N] = {ZERO_LIST(N)};                                     \
    op_arg args[N] = {ARG_ARR_LIST(N)};                                 \
    int halo = 0;                                                       \
    int n_upper, rank;                                                  \
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);                              \
    if (rank == 10) printf ("before if\n");                             \
    if ( set->size == 0 ) {                                             \
      n_upper = op_mpi_halo_exchanges_seq (set, N, args);               \
      op_mpi_wait_all_seq (N,args);                                     \
      op_mpi_set_dirtybit (N, args);                                    \
      REDUCE_LIST(N)                                                    \
      return;                                                           \
    }                                                                   \
    if (rank == 10) printf ("before allocating pointers\n");            \
    ALLOC_POINTER_LIST(N)                                               \
    if (rank == 10) printf ("before halo exch\n");                      \
    n_upper = op_mpi_halo_exchanges_seq (set, N, args);                 \
    if (rank == 10) printf ("before for loop, n_upper = %d\n", n_upper);\
    for ( int n=0; n<n_upper; n++ ) {                                   \
      if ( n==set->core_size ) {                                        \
        if (rank == 10) printf ("before wait all\n");                   \
        op_mpi_wait_all_seq (N,args);                                   \
      }                                                                 \
      if ( n==set->size) halo = 1;                                      \
      ARG_SET_LIST(N);                                                  \
      (*kernel)(PTR_LIST(N));                                           \
    }                                                                   \
    if (rank == 10) printf ("before set dirt bit\n");                   \
    op_mpi_set_dirtybit (N, args);                                      \
    if (rank == 10) printf ("before reduce\n");                         \
    REDUCE_LIST(N)                                                      \
    if (rank == 10) printf ("before free\n");                           \
    FREE_LIST(N)                                                        \
 }

/*    printf ("before set dirt bit\n");                                   \
    printf ("before allocating pointers\n");                            \

    printf ("before reduce\n");                                         \
    printf ("before free list\n");                                      \

//      printf ("before wait all\n");                                     \
//      printf ("before argset list\n");                                  \
//      printf ("at the end\n");                                          \
    printf ("after halo exchanges\n");                                  \

//    printf ("execution size = %d: set->size = %d, set->exec_size = %d \n", n_upper,set->size,set->exec_size); \
//    fflush (0);                                                         \

*/

OP_LOOP(1)  OP_LOOP(2)  OP_LOOP(3)  OP_LOOP(4)  OP_LOOP(5)  OP_LOOP(6)  OP_LOOP(7)  OP_LOOP(8)  OP_LOOP(9)  OP_LOOP(10)
OP_LOOP(11) OP_LOOP(12) OP_LOOP(13) OP_LOOP(14) OP_LOOP(15) OP_LOOP(16) OP_LOOP(17) OP_LOOP(18) OP_LOOP(19) OP_LOOP(20)
OP_LOOP(21) OP_LOOP(22) OP_LOOP(23) OP_LOOP(24) OP_LOOP(25) OP_LOOP(26) OP_LOOP(27) OP_LOOP(28) OP_LOOP(29) OP_LOOP(30)
OP_LOOP(31) OP_LOOP(32) OP_LOOP(33) OP_LOOP(34) OP_LOOP(35) OP_LOOP(36) OP_LOOP(37) OP_LOOP(38) OP_LOOP(39) OP_LOOP(40)
OP_LOOP(41) OP_LOOP(42)
