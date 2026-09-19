// Level 3 of Basic Linear Algebra Subprogram.
#include <utensils/blas.h>

void uts_cblas_dgemm(UTS_ORDER order, UTS_TRANSPOSE transA, UTS_TRANSPOSE transB, int m, int n, int k, double alpha, const double* A, int ldA, const double* B, int ldB, double beta, double* C, int ldC) {
    if (m <= 0 || n <= 0 || k <= 0) return;

    
}