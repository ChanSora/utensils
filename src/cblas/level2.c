// Level 2 of Basic Linear Algebra Subprogram.
#include <utensils/blas.h>

void uts_cblas_dgemv(UTS_ORDER order, UTS_TRANSPOSE trans, int m, int n, double alpha, const double* A, int ldA, const double* x, int incx, double beta, double* y, int incy) {
    if (m <= 0 || n <= 0) return;

    const double* xp, *x_start;
    double* yp = y;
    int lenx, leny;

    if (trans == UtsNoTrans) lenx = n, leny = m;
    if (trans == UtsTrans) lenx = m, leny = n;

    x_start = x;
    if (incx < 0) x_start = x + (-incx) * (lenx - 1);
    if (incy < 0) yp = y + (-incy) * (leny - 1);
    xp = x_start;

    if (order == UtsRowMajor && trans == UtsNoTrans
    || order == UtsColMajor && trans == UtsTrans) {
        for (int i = 0; i < leny; i++) {
            xp = x_start;
            if (beta == 0.0) *yp = 0;
            else *yp *= beta;
            for (int j = 0; j < lenx; j++) {
                *yp += alpha * A[i * ldA + j] * (*xp);
                xp += incx;
            }
            yp += incy;
        }
    } else if (order == UtsColMajor && trans == UtsNoTrans
    || order == UtsRowMajor && trans == UtsTrans) {
        for (int i = 0; i < leny; i++) {
            xp = x_start;
            if (beta == 0.0) *yp = 0;
            else *yp *= beta;
            for (int j = 0; j < lenx; j++) {
                *yp += alpha * A[i + j * ldA] * (*xp);
                xp += incx;
            }
            yp += incy;
        }
    } else return;
}