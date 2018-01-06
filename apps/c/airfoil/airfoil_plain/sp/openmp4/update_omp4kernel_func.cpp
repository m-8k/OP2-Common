//
// auto-generated by op2.py
//

void update_omp4_kernel(
  float *data0,
  int dat0size,
  float *data1,
  int dat1size,
  float *data2,
  int dat2size,
  float *data3,
  int dat3size,
  float *arg4,
  int count,
  int num_teams,
  int nthread,
  int direct_update_stride_OP2CONSTANT){

  float arg4_l = *arg4;
  #pragma omp target teams num_teams(num_teams) thread_limit(nthread) map(to:data0[0:dat0size],data1[0:dat1size],data2[0:dat2size],data3[0:dat3size])\
    map(tofrom: arg4_l) reduction(+:arg4_l)
  #pragma omp distribute parallel for schedule(static,1) reduction(+:arg4_l)
  for ( int n_op=0; n_op<count; n_op++ ){
    //variable mapping
    const float *qold = &data0[n_op];
    float *q = &data1[n_op];
    float *res = &data2[n_op];
    const float *adt = &data3[1*n_op];
    float *rms = &arg4_l;

    //inline function
    
    float del, adti;

    adti = 1.0f / (*adt);

    for (int n = 0; n < 4; n++) {
      del = adti * res[(n)*direct_update_stride_OP2CONSTANT];
      q[(n)*direct_update_stride_OP2CONSTANT] = qold[(n)*direct_update_stride_OP2CONSTANT] - del;
      res[(n)*direct_update_stride_OP2CONSTANT] = 0.0f;
      *rms += del * del;
    }
    //end inline func
  }

  *arg4 = arg4_l;
}
