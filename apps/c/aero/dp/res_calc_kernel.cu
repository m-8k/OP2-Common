//
// auto-generated by op2.m on 19-May-2012 18:43:29
//

// user function

__device__
#include "res_calc.h"


// CUDA kernel function

__global__ void op_cuda_res_calc(
  double *ind_arg0,
  double *ind_arg1,
  double *ind_arg2,
  int   *ind_map,
  short *arg_map,
  double *arg8,
  int   *ind_arg_sizes,
  int   *ind_arg_offs,
  int    block_offset,
  int   *blkmap,
  int   *offset,
  int   *nelems,
  int   *ncolors,
  int   *colors,
  int   nblocks,
  int   set_size) {

  double arg9_l[1];
  double arg10_l[1];
  double arg11_l[1];
  double arg12_l[1];
  double *arg0_vec[4];
  double *arg1_vec[4];
  double *arg2_vec[4] = {
    arg9_l,
    arg10_l,
    arg11_l,
    arg12_l
  };

  __shared__ int   *ind_arg0_map, ind_arg0_size;
  __shared__ int   *ind_arg1_map, ind_arg1_size;
  __shared__ int   *ind_arg2_map, ind_arg2_size;
  __shared__ double *ind_arg0_s;
  __shared__ double *ind_arg1_s;
  __shared__ double *ind_arg2_s;
  __shared__ int    nelems2, ncolor;
  __shared__ int    nelem, offset_b;

  extern __shared__ char shared[];

  if (blockIdx.x+blockIdx.y*gridDim.x >= nblocks) return;
  if (threadIdx.x==0) {

    // get sizes and shift pointers and direct-mapped data

    int blockId = blkmap[blockIdx.x + blockIdx.y*gridDim.x  + block_offset];

    nelem    = nelems[blockId];
    offset_b = offset[blockId];

    nelems2  = blockDim.x*(1+(nelem-1)/blockDim.x);
    ncolor   = ncolors[blockId];

    ind_arg0_size = ind_arg_sizes[0+blockId*3];
    ind_arg1_size = ind_arg_sizes[1+blockId*3];
    ind_arg2_size = ind_arg_sizes[2+blockId*3];

    ind_arg0_map = &ind_map[0*set_size] + ind_arg_offs[0+blockId*3];
    ind_arg1_map = &ind_map[4*set_size] + ind_arg_offs[1+blockId*3];
    ind_arg2_map = &ind_map[8*set_size] + ind_arg_offs[2+blockId*3];

    // set shared memory pointers

    int nbytes = 0;
    ind_arg0_s = (double *) &shared[nbytes];
    nbytes    += ROUND_UP(ind_arg0_size*sizeof(double)*2);
    ind_arg1_s = (double *) &shared[nbytes];
    nbytes    += ROUND_UP(ind_arg1_size*sizeof(double)*1);
    ind_arg2_s = (double *) &shared[nbytes];
  }

  __syncthreads(); // make sure all of above completed

  // copy indirect datasets into shared memory or zero increment

  for (int n=threadIdx.x; n<ind_arg0_size*2; n+=blockDim.x)
    ind_arg0_s[n] = ind_arg0[n%2+ind_arg0_map[n/2]*2];

  for (int n=threadIdx.x; n<ind_arg1_size*1; n+=blockDim.x)
    ind_arg1_s[n] = ind_arg1[n%1+ind_arg1_map[n/1]*1];

  for (int n=threadIdx.x; n<ind_arg2_size*1; n+=blockDim.x)
    ind_arg2_s[n] = ZERO_double;

  __syncthreads();

  // process set elements

  for (int n=threadIdx.x; n<nelems2; n+=blockDim.x) {
    int col2 = -1;

    if (n<nelem) {

      // initialise local variables

      for (int d=0; d<1; d++)
        arg9_l[d] = ZERO_double;
      for (int d=0; d<1; d++)
        arg10_l[d] = ZERO_double;
      for (int d=0; d<1; d++)
        arg11_l[d] = ZERO_double;
      for (int d=0; d<1; d++)
        arg12_l[d] = ZERO_double;

        arg0_vec[0] = ind_arg0_s+arg_map[0*set_size+n+offset_b]*2;
        arg0_vec[1] = ind_arg0_s+arg_map[1*set_size+n+offset_b]*2;
        arg0_vec[2] = ind_arg0_s+arg_map[2*set_size+n+offset_b]*2;
        arg0_vec[3] = ind_arg0_s+arg_map[3*set_size+n+offset_b]*2;

        arg1_vec[0] = ind_arg1_s+arg_map[4*set_size+n+offset_b]*1;
        arg1_vec[1] = ind_arg1_s+arg_map[5*set_size+n+offset_b]*1;
        arg1_vec[2] = ind_arg1_s+arg_map[6*set_size+n+offset_b]*1;
        arg1_vec[3] = ind_arg1_s+arg_map[7*set_size+n+offset_b]*1;
      // user-supplied kernel call


      res_calc(  arg0_vec,
                 arg1_vec,
                 arg8+(n+offset_b),
                 arg2_vec);

      col2 = colors[n+offset_b];
    }

    // store local variables

      int arg9_map;
      int arg10_map;
      int arg11_map;
      int arg12_map;

      if (col2>=0) {
        arg9_map = arg_map[8*set_size+n+offset_b];
        arg10_map = arg_map[9*set_size+n+offset_b];
        arg11_map = arg_map[10*set_size+n+offset_b];
        arg12_map = arg_map[11*set_size+n+offset_b];
      }

    for (int col=0; col<ncolor; col++) {
      if (col2==col) {
        for (int d=0; d<1; d++)
          ind_arg2_s[d+arg9_map*1] += arg9_l[d];
        for (int d=0; d<1; d++)
          ind_arg2_s[d+arg10_map*1] += arg10_l[d];
        for (int d=0; d<1; d++)
          ind_arg2_s[d+arg11_map*1] += arg11_l[d];
        for (int d=0; d<1; d++)
          ind_arg2_s[d+arg12_map*1] += arg12_l[d];
      }
      __syncthreads();
    }

  }

  // apply pointered write/increment

  for (int n=threadIdx.x; n<ind_arg2_size*1; n+=blockDim.x)
    ind_arg2[n%1+ind_arg2_map[n/1]*1] += ind_arg2_s[n];

}


// host stub function

void op_par_loop_res_calc(char const *name, op_set set,
  op_arg arg0,
  op_arg arg4,
  op_arg arg8,
  op_arg arg9 ){


  int    nargs   = 13;
  op_arg args[13];

  arg0.idx = 0;
  args[0] = arg0;
  for (int v = 1; v < 4; v++) {
    args[0 + v] = op_arg_dat(arg0.dat, 0, arg0.map, 2, "double", OP_READ);
  }
  arg4.idx = 0;
  args[4] = arg4;
  for (int v = 1; v < 4; v++) {
    args[4 + v] = op_arg_dat(arg4.dat, 0, arg4.map, 1, "double", OP_READ);
  }
  args[8] = arg8;
  arg9.idx = 0;
  args[9] = arg9;
  for (int v = 1; v < 4; v++) {
    args[9 + v] = op_arg_dat(arg9.dat, 0, arg9.map, 1, "double", OP_INC);
  }

  int    ninds   = 3;
  int    inds[13] = {0,0,0,0,1,1,1,1,-1,2,2,2,2};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: res_calc\n");
  }

  // get plan

  #ifdef OP_PART_SIZE_0
    int part_size = OP_PART_SIZE_0;
  #else
    int part_size = OP_part_size;
  #endif

  int set_size = op_mpi_halo_exchanges(set, nargs, args);

  // initialise timers

  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timers_core(&cpu_t1, &wall_t1);

  if (set->size >0) {

    int op2_stride = set->size + set->exec_size + set->nonexec_size;
    op_decl_const_char(1, "int", sizeof(int), (char *)&op2_stride, "op2_stride");

    op_plan *Plan = op_plan_get(name,set,part_size,nargs,args,ninds,inds);


    // execute plan

    int block_offset = 0;

    for (int col=0; col < Plan->ncolors; col++) {

      if (col==Plan->ncolors_core) op_mpi_wait_all(nargs,args);

    #ifdef OP_BLOCK_SIZE_0
      int nthread = OP_BLOCK_SIZE_0;
    #else
      int nthread = OP_block_size;
    #endif

      dim3 nblocks = dim3(Plan->ncolblk[col] >= (1<<16) ? 65535 : Plan->ncolblk[col],
                      Plan->ncolblk[col] >= (1<<16) ? (Plan->ncolblk[col]-1)/65535+1: 1, 1);
      if (Plan->ncolblk[col] > 0) {
        int nshared = Plan->nsharedCol[col];
        op_cuda_res_calc<<<nblocks,nthread,nshared>>>(
           (double *)arg0.data_d,
           (double *)arg4.data_d,
           (double *)arg9.data_d,
           Plan->ind_map,
           Plan->loc_map,
           (double *)arg8.data_d,
           Plan->ind_sizes,
           Plan->ind_offs,
           block_offset,
           Plan->blkmap,
           Plan->offset,
           Plan->nelems,
           Plan->nthrcol,
           Plan->thrcol,
           Plan->ncolblk[col],
           set_size);

        cutilSafeCall(cudaThreadSynchronize());
        cutilCheckMsg("op_cuda_res_calc execution failed\n");
      }

      block_offset += Plan->ncolblk[col];
    }

    op_timing_realloc(0);
    OP_kernels[0].transfer  += Plan->transfer;
    OP_kernels[0].transfer2 += Plan->transfer2;

  }


  op_mpi_set_dirtybit(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  op_timing_realloc(0);
  OP_kernels[0].name      = name;
  OP_kernels[0].count    += 1;
  OP_kernels[0].time     += wall_t2 - wall_t1;
}

