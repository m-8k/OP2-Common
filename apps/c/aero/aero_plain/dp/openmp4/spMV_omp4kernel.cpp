//
// auto-generated by op2.py
//

//user function
//user function

void spMV_omp4_kernel(int *map0, int map0size, double *data4, int dat4size,
                      double *data0, int dat0size, double *data5, int dat5size,
                      int *col_reord, int set_size1, int start, int end,
                      int num_teams, int nthread);

// host stub function
void op_par_loop_spMV(char const *name, op_set set,
  op_arg arg0,
  op_arg arg4,
  op_arg arg5){

  int nargs = 9;
  op_arg args[9];

  arg0.idx = 0;
  args[0] = arg0;
  for ( int v=1; v<4; v++ ){
    args[0 + v] = op_arg_dat(arg0.dat, v, arg0.map, 1, "double", OP_INC);
  }

  args[4] = arg4;
  arg5.idx = 0;
  args[5] = arg5;
  for ( int v=1; v<4; v++ ){
    args[5 + v] = op_arg_dat(arg5.dat, v, arg5.map, 1, "double", OP_READ);
  }


  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(3);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[3].name      = name;
  OP_kernels[3].count    += 1;

  int  ninds   = 2;
  int  inds[9] = {0,0,0,0,-1,1,1,1,1};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: spMV\n");
  }

  // get plan
  int set_size = op_mpi_halo_exchanges_cuda(set, nargs, args);

  #ifdef OP_PART_SIZE_3
    int part_size = OP_PART_SIZE_3;
  #else
    int part_size = OP_part_size;
  #endif
  #ifdef OP_BLOCK_SIZE_3
    int nthread = OP_BLOCK_SIZE_3;
  #else
    int nthread = OP_block_size;
  #endif


  int ncolors = 0;
  int set_size1 = set->size + set->exec_size;

  if (set_size > 0) {

    //Set up typed device pointers for OpenMP
    int *map0 = arg0.map_data_d;
     int map0size = arg0.map->dim * set_size1;

    double* data4 = (double*)arg4.data_d;
    int dat4size = getSetSizeFromOpArg(&arg4) * arg4.dat->dim;
    double *data0 = (double *)arg0.data_d;
    int dat0size = getSetSizeFromOpArg(&arg0) * arg0.dat->dim;
    double *data5 = (double *)arg5.data_d;
    int dat5size = getSetSizeFromOpArg(&arg5) * arg5.dat->dim;

    op_plan *Plan = op_plan_get_stage(name,set,part_size,nargs,args,ninds,inds,OP_COLOR2);
    ncolors = Plan->ncolors;
    int *col_reord = Plan->col_reord;

    // execute plan
    for ( int col=0; col<Plan->ncolors; col++ ){
      if (col==1) {
        op_mpi_wait_all_cuda(nargs, args);
      }
      int start = Plan->col_offsets[0][col];
      int end = Plan->col_offsets[0][col+1];

      spMV_omp4_kernel(map0, map0size, data4, dat4size, data0, dat0size, data5,
                       dat5size, col_reord, set_size1, start, end,
                       part_size != 0 ? (end - start - 1) / part_size + 1
                                      : (end - start - 1) / nthread,
                       nthread);
    }
    OP_kernels[3].transfer  += Plan->transfer;
    OP_kernels[3].transfer2 += Plan->transfer2;
  }

  if (set_size == 0 || set_size == set->core_size || ncolors == 1) {
    op_mpi_wait_all_cuda(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit_cuda(nargs, args);

  if (OP_diags>1) deviceSync();
  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[3].time     += wall_t2 - wall_t1;
}
