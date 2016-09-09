//
// auto-generated by op2.py
//

void adt_calc_omp4_kernel(
  int *map0,
  double *data4,
  double *data5,
  double *data0,
  int *col_reord,
  int set_size1,
  int start,
  int end,
  int num_teams,
  int nthread){

  #pragma omp target teams num_teams(num_teams) thread_limit(nthread) map(to:data4,data5) \
    map(to: gam_ompkernel, gm1_ompkernel, cfl_ompkernel)\
    map(to:col_reord,map0,data0)
  #pragma omp distribute parallel for schedule(static,1)
  for ( int e=start; e<end; e++ ){
    int n_op = col_reord[e];
    int map0idx = map0[n_op + set_size1 * 0];
    int map1idx = map0[n_op + set_size1 * 1];
    int map2idx = map0[n_op + set_size1 * 2];
    int map3idx = map0[n_op + set_size1 * 3];
    //variable mapping
    const double *x1 = &data0[2 * map0idx];
    const double *x2 = &data0[2 * map1idx];
    const double *x3 = &data0[2 * map2idx];
    const double *x4 = &data0[2 * map3idx];
    const double *q = &data4[4*n_op];
    double *adt = &data5[1*n_op];

    //inline function
      
    double dx, dy, ri, u, v, c;
  
    ri = 1.0f / q[0];
    u = ri * q[1];
    v = ri * q[2];
    c = sqrt(gam_ompkernel * gm1_ompkernel * (ri * q[3] - 0.5f * (u * u + v * v)));
  
    dx = x2[0] - x1[0];
    dy = x2[1] - x1[1];
    *adt = fabs(u * dy - v * dx) + c * sqrt(dx * dx + dy * dy);
  
    dx = x3[0] - x2[0];
    dy = x3[1] - x2[1];
    *adt += fabs(u * dy - v * dx) + c * sqrt(dx * dx + dy * dy);
  
    dx = x4[0] - x3[0];
    dy = x4[1] - x3[1];
    *adt += fabs(u * dy - v * dx) + c * sqrt(dx * dx + dy * dy);
  
    dx = x1[0] - x4[0];
    dy = x1[1] - x4[1];
    *adt += fabs(u * dy - v * dx) + c * sqrt(dx * dx + dy * dy);
  
    *adt = (*adt) * (1.0f / cfl_ompkernel);
    //end inline func
  }

}
