//
// auto-generated by op2.m on 16-Oct-2012 15:15:09
//

// user function

#include "update.h"


// x86 kernel function

void op_x86_update(
  double *arg0,
  double *arg1,
  double *arg2,
  double *arg3,
  double *arg4,
  int   start,
  int   finish ) {


  // process set elements

  for (int n=start; n<finish; n++) {

    // user-supplied kernel call


    update(  arg0+n*4,
             arg1+n*4,
             arg2+n*4,
             arg3+n*1,
             arg4 );
  }
}


// host stub function

void op_par_loop_update(op_kernel_descriptor *desc ){

  char const *name = desc->name;
  op_set set = desc->set;
  op_arg arg0 = desc->args[0];
  op_arg arg1 = desc->args[1];
  op_arg arg2 = desc->args[2];
  op_arg arg3 = desc->args[3];
  op_arg arg4 = desc->args[4];

  double *arg4h = (double *)arg4.data;

  int    nargs   = 5;
  op_arg args[5];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;

  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  update\n");
  }

  op_mpi_halo_exchanges(set, nargs, args);

  // initialise timers

  double cpu_t1, cpu_t2, wall_t1=0, wall_t2=0;
  op_timing_realloc(4);
  OP_kernels[4].name      = name;
  OP_kernels[4].count    += 1;

  // set number of threads

#ifdef _OPENMP
  int nthreads = omp_get_max_threads( );
#else
  int nthreads = 1;
#endif

  // allocate and initialise arrays for global reduction

  double arg4_l[1+64*64];
  for (int thr=0; thr<nthreads; thr++)
    for (int d=0; d<1; d++) arg4_l[d+thr*64]=ZERO_double;

  if (set->size >0) {

    op_timers_core(&cpu_t1, &wall_t1);

  // execute plan

#pragma omp parallel for
  for (int thr=0; thr<nthreads; thr++) {
    int start  = (set->size* thr   )/nthreads;
    int finish = (set->size*(thr+1))/nthreads;
    op_x86_update( (double *) arg0.data,
                   (double *) arg1.data,
                   (double *) arg2.data,
                   (double *) arg3.data,
                   arg4_l + thr*64,
                   start, finish );
  }

  }


  // combine reduction data

  for (int thr=0; thr<nthreads; thr++)
    for(int d=0; d<1; d++) arg4h[d] += arg4_l[d+thr*64];

  op_mpi_reduce(&arg4,arg4h);

  op_mpi_set_dirtybit(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[4].time     += wall_t2 - wall_t1;
  OP_kernels[4].transfer += (float)set->size * arg0.size;
  OP_kernels[4].transfer += (float)set->size * arg1.size;
  OP_kernels[4].transfer += (float)set->size * arg2.size * 2.0f;
  OP_kernels[4].transfer += (float)set->size * arg3.size;
}

void op_par_loop_update_enqueue(char const *name, op_set set,
                                   op_arg arg0,
                                   op_arg arg1,
                                   op_arg arg2,
                                   op_arg arg3,
                                   op_arg arg4) {
  op_kernel_descriptor kern;
  kern.name = name;
  kern.set = set;
  kern.args = (op_arg *)malloc(5*sizeof(op_arg));
  kern.args[0] = arg0;
  kern.args[1] = arg1;
  kern.args[2] = arg2;
  kern.args[3] = arg3;
  kern.args[4] = arg4;
  kern.function = op_par_loop_update;
  kernel_list.push_back(kern);
}