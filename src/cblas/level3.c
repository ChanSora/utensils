// Level 3 of Basic Linear Algebra Subprogram.
#include <utensils/blas.h>

void uts_cblas_dgemm(UTS_ORDER order, UTS_TRANSPOSE transA, UTS_TRANSPOSE transB, int m, int n, int k, double alpha, const double* A, int ldA, const double* B, int ldB, double beta, double* C, int ldC) {
    if (m <= 0 || n <= 0) return;

    if (k <= 0 || alpha == 0.0) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (order == UtsRowMajor) {
                    if (beta == 0.0) C[i * ldC + j] = 0.0;
                    else C[i * ldC + j] *= beta;
                } else {
                    if (beta == 0.0) C[i + j * ldC] = 0.0;
                    else C[i + j * ldC] *= beta;
                }
            }
        }
        return;
    }

    if (order == UtsRowMajor && transA == UtsNoTrans && transB == UtsNoTrans
    || order == UtsColMajor && transA == UtsTrans && transB == UtsTrans) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (order == UtsRowMajor) {
                    if (beta == 0.0) C[i * ldC + j] = 0.0;
                    else C[i * ldC + j] *= beta;
                    for (int u = 0; u < k; u++) {
                        C[i * ldC + j] += alpha * A[i * ldA + u] * B[u * ldB + j];
                    } 
                } else {
                    if (beta == 0.0) C[i + j * ldC] = 0.0;
                    else C[i + j * ldC] *= beta;
                    for (int u = 0; u < k; u++) {
                        C[i + j * ldC] += alpha * A[i * ldA + u] * B[u * ldB + j];
                    } 
                }
            }
        }
    } else if (order == UtsRowMajor && transA == UtsNoTrans && transB == UtsTrans
    || order == UtsColMajor && transA == UtsTrans && transB == UtsNoTrans) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (order == UtsRowMajor) {
                    if (beta == 0.0) C[i * ldC + j] = 0.0;
                    else C[i * ldC + j] *= beta;
                    for (int u = 0; u < k; u++) {
                        C[i * ldC + j] += alpha * A[i * ldA + u] * B[j * ldB + u];
                    } 
                } else {
                    if (beta == 0.0) C[i + j * ldC] = 0.0;
                    else C[i + j * ldC] *= beta;
                    for (int u = 0; u < k; u++) {
                        C[i + j * ldC] += alpha * A[i * ldA + u] * B[j * ldB + u];
                    } 
                }
            }
        }
    } else if (order == UtsRowMajor && transA == UtsTrans && transB == UtsNoTrans
    || order == UtsColMajor && transA == UtsNoTrans && transB == UtsTrans) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (order == UtsRowMajor) {
                    if (beta == 0.0) C[i * ldC + j] = 0.0;
                    else C[i * ldC + j] *= beta;
                    for (int u = 0; u < k; u++) {
                        C[i * ldC + j] += alpha * A[u * ldA + i] * B[u * ldB + j];
                    }
                } else {
                    if (beta == 0.0) C[i + j * ldC] = 0.0;
                    else C[i + j * ldC] *= beta;
                    for (int u = 0; u < k; u++) {
                        C[i + j * ldC] += alpha * A[u * ldA + i] * B[u * ldB + j];
                    }
                }
            }
        }
    } else if (order == UtsRowMajor && transA == UtsTrans && transB == UtsTrans
    || order == UtsColMajor && transA == UtsNoTrans && transB == UtsNoTrans) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (order == UtsRowMajor) {
                    if (beta == 0.0) C[i * ldC + j] = 0.0;
                    else C[i * ldC + j] *= beta;
                    for (int u = 0; u < k; u++) {
                        C[i * ldC + j] += alpha * A[u * ldA + i] * B[j * ldB + u];
                    }
                } else {
                    if (beta == 0.0) C[i + j * ldC] = 0.0;
                    else C[i + j * ldC] *= beta;
                    for (int u = 0; u < k; u++) {
                        C[i + j * ldC] += alpha * A[u * ldA + i] * B[j * ldB + u];
                    }
                }
            }
        }
    } else return;
}