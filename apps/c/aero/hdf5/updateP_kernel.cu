//
// auto-generated by op2.m on 19-May-2012 18:43:34
//

// user function

__device__
#include "updateP.h"


// CUDA kernel function

__global__ void op_cuda_updateP(
  double *arg0,
  double *arg1,
  const double *arg2,
  int   offset_s,
  int   set_size ) {


  // process set elements

  for (int n=threadIdx.x+blockIdx.x*blockDim.x;
       n<set_size; n+=blockDim.x*gridDim.x) {
    // user-supplied kernel call


    updateP(  arg0+n,
              arg1+n,
              arg2 );
  }
}


// host stub function

void op_par_loop_updateP(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2 ){

  double *arg2h = (double *)arg2.data;

  int    nargs   = 3;
  op_arg args[3];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;

  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  updateP\n");
  }

  op_mpi_halo_exchanges(set, nargs, args);

  // initialise timers

  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timers_core(&cpu_t1, &wall_t1);

  if (set->size >0) {


    // transfer constants to GPU

    int consts_bytes = 0;
    consts_bytes += ROUND_UP(1*sizeof(double));

    reallocConstArrays(consts_bytes);

    consts_bytes = 0;
    arg2.data   = OP_consts_h + consts_bytes;
    arg2.data_d = OP_consts_d + consts_bytes;
    for (int d=0; d<1; d++) ((double *)arg2.data)[d] = arg2h[d];
    consts_bytes += ROUND_UP(1*sizeof(double));

    mvConstArraysToDevice(consts_bytes);

    // set CUDA execution parameters

    #ifdef OP_BLOCK_SIZE_7
      int nthread = OP_BLOCK_SIZE_7;
    #else
      // int nthread = OP_block_size;
      int nthread = 128;
    #endif

    int nblocks = 200;

    // work out shared memory requirements per element

    int nshared = 0;

    // execute plan

    int offset_s = nshared*OP_WARPSIZE;

    nshared = nshared*nthread;

    op_cuda_updateP<<<nblocks,nthread,nshared>>>( (double *) arg0.data_d,
                                                  (double *) arg1.data_d,
                                                  (double *) arg2.data_d,
                                                  offset_s,
                                                  set->size );

    cutilSafeCall(cudaThreadSynchronize());
    cutilCheckMsg("op_cuda_updateP execution failed\n");

  }


  op_mpi_set_dirtybit(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  op_timing_realloc(7);
  OP_kernels[7].name      = name;
  OP_kernels[7].count    += 1;
  OP_kernels[7].time     += wall_t2 - wall_t1;
  OP_kernels[7].transfer += (float)set->size * arg0.size;
  OP_kernels[7].transfer += (float)set->size * arg1.size * 2.0f;
}

