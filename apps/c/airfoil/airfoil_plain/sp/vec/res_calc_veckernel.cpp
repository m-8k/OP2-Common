//
// auto-generated by op2.py
//

//user function
inline void res_calc(const float *x1, const float *x2, const float *q1,
                     const float *q2, const float *adt1, const float *adt2,
                     float *res1, float *res2) {
  float dx, dy, mu, ri, p1, vol1, p2, vol2, f;

  dx = x1[0] - x2[0];
  dy = x1[1] - x2[1];

  ri = 1.0f / q1[0];
  p1 = gm1 * (q1[3] - 0.5f * ri * (q1[1] * q1[1] + q1[2] * q1[2]));
  vol1 = ri * (q1[1] * dy - q1[2] * dx);

  ri = 1.0f / q2[0];
  p2 = gm1 * (q2[3] - 0.5f * ri * (q2[1] * q2[1] + q2[2] * q2[2]));
  vol2 = ri * (q2[1] * dy - q2[2] * dx);

  mu = 0.5f * ((*adt1) + (*adt2)) * eps;

  f = 0.5f * (vol1 * q1[0] + vol2 * q2[0]) + mu * (q1[0] - q2[0]);
  res1[0] += f;
  res2[0] -= f;
  f = 0.5f * (vol1 * q1[1] + p1 * dy + vol2 * q2[1] + p2 * dy) +
      mu * (q1[1] - q2[1]);
  res1[1] += f;
  res2[1] -= f;
  f = 0.5f * (vol1 * q1[2] - p1 * dx + vol2 * q2[2] - p2 * dx) +
      mu * (q1[2] - q2[2]);
  res1[2] += f;
  res2[2] -= f;
  f = 0.5f * (vol1 * (q1[3] + p1) + vol2 * (q2[3] + p2)) + mu * (q1[3] - q2[3]);
  res1[3] += f;
  res2[3] -= f;
}
#ifdef VECTORIZE
//user function -- modified for vectorisation
#if defined __clang__ || defined __GNUC__
__attribute__((always_inline))
#endif
inline void
res_calc_vec(const float x1[][SIMD_VEC], const float x2[][SIMD_VEC],
             const float q1[][SIMD_VEC], const float q2[][SIMD_VEC],
             const float adt1[][SIMD_VEC], const float adt2[][SIMD_VEC],
             float res1[][SIMD_VEC], float res2[][SIMD_VEC], int idx) {
  float dx, dy, mu, ri, p1, vol1, p2, vol2, f;

  dx = x1[0][idx] - x2[0][idx];
  dy = x1[1][idx] - x2[1][idx];

  ri = 1.0f / q1[0][idx];
  p1 = gm1 * (q1[3][idx] - 0.5f * ri * (q1[1][idx] * q1[1][idx] + q1[2][idx] * q1[2][idx]));
  vol1 = ri * (q1[1][idx] * dy - q1[2][idx] * dx);

  ri = 1.0f / q2[0][idx];
  p2 = gm1 * (q2[3][idx] - 0.5f * ri * (q2[1][idx] * q2[1][idx] + q2[2][idx] * q2[2][idx]));
  vol2 = ri * (q2[1][idx] * dy - q2[2][idx] * dx);

  mu = 0.5f * ((adt1[0][idx]) + (adt2[0][idx])) * eps;

  f = 0.5f * (vol1 * q1[0][idx] + vol2 * q2[0][idx]) + mu * (q1[0][idx] - q2[0][idx]);
  res1[0][idx] = f;
  res2[0][idx] -= f;
  f = 0.5f * (vol1 * q1[1][idx] + p1 * dy + vol2 * q2[1][idx] + p2 * dy) +
      mu * (q1[1][idx] - q2[1][idx]);
  res1[1][idx] = f;
  res2[1][idx] -= f;
  f = 0.5f * (vol1 * q1[2][idx] - p1 * dx + vol2 * q2[2][idx] - p2 * dx) +
      mu * (q1[2][idx] - q2[2][idx]);
  res1[2][idx] = f;
  res2[2][idx] -= f;
  f = 0.5f * (vol1 * (q1[3][idx] + p1) + vol2 * (q2[3][idx] + p2)) + mu * (q1[3][idx] - q2[3][idx]);
  res1[3][idx] = f;
  res2[3][idx] -= f;

}
#endif

// host stub function
void op_par_loop_res_calc(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4,
  op_arg arg5,
  op_arg arg6,
  op_arg arg7){

  int nargs = 8;
  op_arg args[8];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;
  args[5] = arg5;
  args[6] = arg6;
  args[7] = arg7;
  //create aligned pointers for dats
  ALIGNED_float const float * __restrict__ ptr0 = (float *) arg0.data;
  DECLARE_PTR_ALIGNED(ptr0, float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr1 = (float *) arg1.data;
  DECLARE_PTR_ALIGNED(ptr1, float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr2 = (float *) arg2.data;
  DECLARE_PTR_ALIGNED(ptr2, float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr3 = (float *) arg3.data;
  DECLARE_PTR_ALIGNED(ptr3, float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr4 = (float *) arg4.data;
  DECLARE_PTR_ALIGNED(ptr4, float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr5 = (float *) arg5.data;
  DECLARE_PTR_ALIGNED(ptr5, float_ALIGN);
  ALIGNED_float       float * __restrict__ ptr6 = (float *) arg6.data;
  DECLARE_PTR_ALIGNED(ptr6, float_ALIGN);
  ALIGNED_float       float * __restrict__ ptr7 = (float *) arg7.data;
  DECLARE_PTR_ALIGNED(ptr7, float_ALIGN);

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(2);
  op_timers_core(&cpu_t1, &wall_t1);

  if (OP_diags>2) {
    printf(" kernel routine with indirection: res_calc\n");
  }

  int exec_size = op_mpi_halo_exchanges(set, nargs, args);

  if (exec_size >0) {

    #ifdef VECTORIZE
    #pragma novector
    for ( int n=0; n<(exec_size/SIMD_VEC)*SIMD_VEC; n+=SIMD_VEC ){
      if ((n + SIMD_VEC >= set->core_size) &&
          (n + SIMD_VEC - set->core_size < SIMD_VEC)) {
        op_mpi_wait_all(nargs, args);
      }
      ALIGNED_float float dat0[2][SIMD_VEC];
      ALIGNED_float float dat1[2][SIMD_VEC];
      ALIGNED_float float dat2[4][SIMD_VEC];
      ALIGNED_float float dat3[4][SIMD_VEC];
      ALIGNED_float float dat4[1][SIMD_VEC];
      ALIGNED_float float dat5[1][SIMD_VEC];
      ALIGNED_float float dat6[4][SIMD_VEC];
      ALIGNED_float float dat7[4][SIMD_VEC];
      #pragma omp simd simdlen(SIMD_VEC)
      for ( int i=0; i<SIMD_VEC; i++ ){
        int idx0_2 = 2 * arg0.map_data[(n+i) * arg0.map->dim + 0];
        int idx1_2 = 2 * arg0.map_data[(n+i) * arg0.map->dim + 1];
        int idx2_4 = 4 * arg2.map_data[(n+i) * arg2.map->dim + 0];
        int idx3_4 = 4 * arg2.map_data[(n+i) * arg2.map->dim + 1];
        int idx4_1 = 1 * arg2.map_data[(n+i) * arg2.map->dim + 0];
        int idx5_1 = 1 * arg2.map_data[(n+i) * arg2.map->dim + 1];

        dat0[0][i] = (ptr0)[idx0_2 + 0];
        dat0[1][i] = (ptr0)[idx0_2 + 1];

        dat1[0][i] = (ptr1)[idx1_2 + 0];
        dat1[1][i] = (ptr1)[idx1_2 + 1];

        dat2[0][i] = (ptr2)[idx2_4 + 0];
        dat2[1][i] = (ptr2)[idx2_4 + 1];
        dat2[2][i] = (ptr2)[idx2_4 + 2];
        dat2[3][i] = (ptr2)[idx2_4 + 3];

        dat3[0][i] = (ptr3)[idx3_4 + 0];
        dat3[1][i] = (ptr3)[idx3_4 + 1];
        dat3[2][i] = (ptr3)[idx3_4 + 2];
        dat3[3][i] = (ptr3)[idx3_4 + 3];

        dat4[0][i] = (ptr4)[idx4_1 + 0];

        dat5[0][i] = (ptr5)[idx5_1 + 0];

        dat6[0][i] = 0.0;
        dat6[1][i] = 0.0;
        dat6[2][i] = 0.0;
        dat6[3][i] = 0.0;

        dat7[0][i] = 0.0;
        dat7[1][i] = 0.0;
        dat7[2][i] = 0.0;
        dat7[3][i] = 0.0;

      }
      #pragma omp simd simdlen(SIMD_VEC)
      for ( int i=0; i<SIMD_VEC; i++ ){
        res_calc_vec(
          dat0,
          dat1,
          dat2,
          dat3,
          dat4,
          dat5,
          dat6,
          dat7,
          i);
      }
      for ( int i=0; i<SIMD_VEC; i++ ){
        int idx6_4 = 4 * arg2.map_data[(n+i) * arg2.map->dim + 0];
        int idx7_4 = 4 * arg2.map_data[(n+i) * arg2.map->dim + 1];

        (ptr6)[idx6_4 + 0] += dat6[0][i];
        (ptr6)[idx6_4 + 1] += dat6[1][i];
        (ptr6)[idx6_4 + 2] += dat6[2][i];
        (ptr6)[idx6_4 + 3] += dat6[3][i];

        (ptr7)[idx7_4 + 0] += dat7[0][i];
        (ptr7)[idx7_4 + 1] += dat7[1][i];
        (ptr7)[idx7_4 + 2] += dat7[2][i];
        (ptr7)[idx7_4 + 3] += dat7[3][i];

      }
    }

    //remainder
    for ( int n=(exec_size/SIMD_VEC)*SIMD_VEC; n<exec_size; n++ ){
    #else
    for ( int n=0; n<exec_size; n++ ){
    #endif
      if (n==set->core_size) {
        op_mpi_wait_all(nargs, args);
      }
      int map0idx;
      int map1idx;
      int map2idx;
      int map3idx;
      map0idx = arg0.map_data[n * arg0.map->dim + 0];
      map1idx = arg0.map_data[n * arg0.map->dim + 1];
      map2idx = arg2.map_data[n * arg2.map->dim + 0];
      map3idx = arg2.map_data[n * arg2.map->dim + 1];

      res_calc(
        &(ptr0)[2 * map0idx],
        &(ptr1)[2 * map1idx],
        &(ptr2)[4 * map2idx],
        &(ptr3)[4 * map3idx],
        &(ptr4)[1 * map2idx],
        &(ptr5)[1 * map3idx],
        &(ptr6)[4 * map2idx],
        &(ptr7)[4 * map3idx]);
    }
  }

  if (exec_size == 0 || exec_size == set->core_size) {
    op_mpi_wait_all(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[2].name      = name;
  OP_kernels[2].count    += 1;
  OP_kernels[2].time     += wall_t2 - wall_t1;
  OP_kernels[2].transfer += (float)set->size * arg0.size;
  OP_kernels[2].transfer += (float)set->size * arg2.size;
  OP_kernels[2].transfer += (float)set->size * arg4.size;
  OP_kernels[2].transfer += (float)set->size * arg6.size * 2.0f;
  OP_kernels[2].transfer += (float)set->size * arg0.map->dim * 4.0f;
  OP_kernels[2].transfer += (float)set->size * arg2.map->dim * 4.0f;
}
