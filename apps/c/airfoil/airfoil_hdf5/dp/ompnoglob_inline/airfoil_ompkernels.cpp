//
// auto-generated by op2.py
//

// header
#include "op_lib_cpp.h"

// global constants
/*extern double gam;
extern double gm1;
extern double cfl;
extern double eps;
extern double mach;
extern double alpha;
extern double qinf[4];*/
void op_decl_const_char(int dim, char const *type,
int size, char *dat, char const *name){}
// user kernel files
#include "save_soln_ompkernel.cpp"
#include "adt_calc_ompkernel.cpp"
#include "res_calc_ompkernel.cpp"
#include "bres_calc_ompkernel.cpp"
#include "update_ompkernel.cpp"