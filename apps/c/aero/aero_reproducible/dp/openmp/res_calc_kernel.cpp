//
// auto-generated by op2.py
//

//user function
#include "../res_calc.h"

// host stub function
void op_par_loop_res_calc(char const *name, op_set set,
  op_arg arg0,
  op_arg arg4,
  op_arg arg8,
  op_arg arg9,
  op_arg arg13){

  int nargs = 17;
  op_arg args[17];

  arg0.idx = 0;
  args[0] = arg0;
  for ( int v=1; v<4; v++ ){
    args[0 + v] = op_arg_dat(arg0.dat, v, arg0.map, 2, "double", OP_READ);
  }

  arg4.idx = 0;
  args[4] = arg4;
  for ( int v=1; v<4; v++ ){
    args[4 + v] = op_arg_dat(arg4.dat, v, arg4.map, 1, "double", OP_READ);
  }

  args[8] = arg8;
  arg9.idx = 0;
  args[9] = arg9;
  for ( int v=1; v<4; v++ ){
    args[9 + v] = op_opt_arg_dat(arg9.opt, arg9.dat, v, arg9.map, 1, "double", OP_RW);
  }

  arg13.idx = 0;
  args[13] = arg13;
  for ( int v=1; v<4; v++ ){
    args[13 + v] = op_opt_arg_dat(arg13.opt, arg13.dat, v, arg13.map, 2, "double", OP_INC);
  }


  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(0);
  op_timers_core(&cpu_t1, &wall_t1);

  if (OP_diags>2) {
    printf(" kernel routine with indirection: res_calc\n");
  }

  int set_size = op_mpi_halo_exchanges(set, nargs, args);

  if (set->size >0) {

  

    op_map prime_map = arg9.map; //TODO works only with arg9...
    op_reversed_map rev_map = OP_reversed_map_list[prime_map->index];
    
    op_mpi_wait_all(nargs, args);
    if (rev_map != NULL) {
      /*  int prime_map_dim = prime_map->dim;
        int set_from_size = prime_map->from->size + prime_map->from->exec_size ;
        int set_to_size = prime_map->to->size + prime_map->to->exec_size + prime_map->to->nonexec_size;

        int required_tmp_incs_size = set_from_size * prime_map_dim * arg9.dat->size;
    
        if (op_repr_incs[arg9.dat->index].tmp_incs == NULL){
            op_repr_incs[arg9.dat->index].tmp_incs = (void *)op_malloc(required_tmp_incs_size);
            op_repr_incs[arg9.dat->index].tmp_incs_size = required_tmp_incs_size;
        } else if (op_repr_incs[arg9.dat->index].tmp_incs_size < required_tmp_incs_size){
            op_realloc(op_repr_incs[arg9.dat->index].tmp_incs, required_tmp_incs_size);
            op_repr_incs[arg9.dat->index].tmp_incs_size = required_tmp_incs_size;
        }

        double *tmp_incs = (double *)op_repr_incs[arg9.dat->index].tmp_incs;

        for (int i=0; i<set_from_size * prime_map_dim * arg9.dim; i++){
          tmp_incs[i]=0.0;
        }

*/
        
        for (int c=0; c<rev_map->number_of_colors;c++){
            #pragma omp parallel for
            for ( int i=rev_map->color_based_exec_row_starts[c]; i<rev_map->color_based_exec_row_starts[c+1]; i++ ){
                int n=rev_map->color_based_exec[i];
  //      for ( int n=0; n<set->core_size; n++ ){
  //        if (n==set->core_size) {
  //          op_mpi_wait_all(nargs, args);
  //        }
          int map0idx = arg0.map_data[n * arg0.map->dim + 0];
          int map1idx = arg0.map_data[n * arg0.map->dim + 1];
          int map2idx = arg0.map_data[n * arg0.map->dim + 2];
          int map3idx = arg0.map_data[n * arg0.map->dim + 3];

          const double* arg0_vec[] = {
             &((double*)arg0.data)[2 * map0idx],
             &((double*)arg0.data)[2 * map1idx],
             &((double*)arg0.data)[2 * map2idx],
             &((double*)arg0.data)[2 * map3idx]};
          const double* arg4_vec[] = {
             &((double*)arg4.data)[1 * map0idx],
             &((double*)arg4.data)[1 * map1idx],
             &((double*)arg4.data)[1 * map2idx],
             &((double*)arg4.data)[1 * map3idx]};
          double* arg9_vec[] = {
             &((double*)arg9.data)[1 * map0idx],
             &((double*)arg9.data)[1 * map1idx],
             &((double*)arg9.data)[1 * map2idx],
             &((double*)arg9.data)[1 * map3idx]};
   /*       double* arg9_vec[] = {
             &tmp_incs[(n*prime_map_dim+0)*arg9.dim],
             &tmp_incs[(n*prime_map_dim+1)*arg9.dim],
             &tmp_incs[(n*prime_map_dim+2)*arg9.dim],
             &tmp_incs[(n*prime_map_dim+3)*arg9.dim]};*/
          double* arg13_vec[] = {
             &((double*)arg13.data)[2 * map0idx],
             &((double*)arg13.data)[2 * map1idx],
             &((double*)arg13.data)[2 * map2idx],
             &((double*)arg13.data)[2 * map3idx]};

          res_calc(
            arg0_vec,
            arg4_vec,
            &((double*)arg8.data)[16 * n],
            arg9_vec,
            arg13_vec);
        }
        
  /*      op_mpi_wait_all(nargs, args);
        
        #pragma omp parallel for
        for ( int n=set->core_size; n<set_size; n++ ){
  //        if (n==set->core_size) {
  //          op_mpi_wait_all(nargs, args);
  //        }
          int map0idx = arg0.map_data[n * arg0.map->dim + 0];
          int map1idx = arg0.map_data[n * arg0.map->dim + 1];
          int map2idx = arg0.map_data[n * arg0.map->dim + 2];
          int map3idx = arg0.map_data[n * arg0.map->dim + 3];

          const double* arg0_vec[] = {
             &((double*)arg0.data)[2 * map0idx],
             &((double*)arg0.data)[2 * map1idx],
             &((double*)arg0.data)[2 * map2idx],
             &((double*)arg0.data)[2 * map3idx]};
          const double* arg4_vec[] = {
             &((double*)arg4.data)[1 * map0idx],
             &((double*)arg4.data)[1 * map1idx],
             &((double*)arg4.data)[1 * map2idx],
             &((double*)arg4.data)[1 * map3idx]};
          double* arg9_vec[] = {
             &tmp_incs[(n*prime_map_dim+0)*arg9.dim],
             &tmp_incs[(n*prime_map_dim+1)*arg9.dim],
             &tmp_incs[(n*prime_map_dim+2)*arg9.dim],
             &tmp_incs[(n*prime_map_dim+3)*arg9.dim]};
          double* arg13_vec[] = {
             &((double*)arg13.data)[2 * map0idx],
             &((double*)arg13.data)[2 * map1idx],
             &((double*)arg13.data)[2 * map2idx],
             &((double*)arg13.data)[2 * map3idx]};

          res_calc(
            arg0_vec,
            arg4_vec,
            &((double*)arg8.data)[16 * n],
            arg9_vec,
            arg13_vec);
        }
        
        #pragma omp parallel for
        for ( int n=0; n<set_to_size; n++ ){
            for ( int i=0; i<rev_map->row_start_idx[n+1] - rev_map->row_start_idx[n]; i++){
                for (int d=0; d<arg9.dim; d++){
                    ((double*)arg9.data)[arg9.dim * n + d] +=
                    tmp_incs[rev_map->reversed_map[rev_map->row_start_idx[n]+i] * arg9.dim + d];
                }
            }
        }  */
            }
    }
  }

  if (set_size == 0 || set_size == set->core_size) {
    op_mpi_wait_all(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[0].name      = name;
  OP_kernels[0].count    += 1;
  OP_kernels[0].time     += wall_t2 - wall_t1;
  OP_kernels[0].transfer += (float)set->size * arg0.size;
  OP_kernels[0].transfer += (float)set->size * arg4.size;
  OP_kernels[0].transfer += (float)set->size * arg9.size * 2.0f;
  OP_kernels[0].transfer += (float)set->size * arg13.size * 2.0f;
  if (arg8.opt) {
    OP_kernels[0].transfer += (float)set->size * arg8.size;
  }
  OP_kernels[0].transfer += (float)set->size * arg0.map->dim * 4.0f;
}
