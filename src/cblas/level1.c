// Level 1 of Basic Linear Algebra Subprogram.
#include <math.h>

void uts_cblas_daxpy(int n, double alpha, const double* x, int incx, double* y, int incy) {
    if (n <= 0) return;
    
    const double* xp = x;
    double* yp = y;

    if (incx < 0) xp = x + (-incx) * (n - 1);
    if (incy < 0) yp = y + (-incy) * (n - 1);

    for (int i = 0; i < n; i++) {
        *yp += alpha * (*xp);    
        xp += incx;
        yp += incy;
    }
}

void uts_cblas_dscal(int n, double alpha, double* x, int incx) {
    if (n <= 0) return;
    
    double* xp = x;

    if (incx < 0) xp = x + (-incx) * (n - 1);

    for (int i = 0; i < n; i++) {
        *xp *= alpha;
        xp += incx;
    }
}

void uts_cblas_dcopy(int n, const double* x, int incx, double* y, int incy) {
    if (n <= 0) return;

    const double* xp = x;
    double* yp = y;

    if (incx < 0) xp = x + (-incx) * (n - 1);
    if (incy < 0) yp = y + (-incy) * (n - 1);

    for (int i = 0; i < n; i++) {
        *yp = (*xp);    
        xp += incx;
        yp += incy;
    }
}

void uts_cblas_dswap(int n, double* x, int incx, double* y, int incy) {
    if (n <= 0) return;

    double* xp = x;
    double* yp = y;

    if (incx < 0) xp = x + (-incx) * (n - 1);
    if (incy < 0) yp = y + (-incy) * (n - 1);

    for (int i = 0; i < n; i++) {
        double tmp = *xp;
        *xp = (*yp);
        *yp = tmp;
        xp += incx;
        yp += incy;
    }
}

double uts_cblas_ddot(int n, const double* x, int incx, const double* y, int incy) {
    if (n <= 0) return 0.0;
    
    const double* xp = x;
    const double* yp = y;
    double res = 0.0;

    if (incx < 0) xp = x + (-incx) * (n - 1);
    if (incy < 0) yp = y + (-incy) * (n - 1);

    for (int i = 0; i < n; i++) {
        res += (*xp) * (*yp);
        xp += incx;
        yp += incy;
    }
    return res;
}

double uts_cblas_dnrm2(int n, const double* x, int incx) {
    if (n <= 0 || incx <= 0) return 0.0;

    const double* xp = x;

    double scale = 0.0;

    for (int i = 0; i < n; i++) {
        double ax = fabs((*xp));
        if (ax > scale) scale = ax;
        xp += incx;
    }

    if (scale == 0.0) return 0.0;

    xp = x;

    double ssq = 0.0;

    for (int i = 0; i < n; i++) {
        double t = (*xp) / scale;
        ssq += t * t;
        xp += incx;
    }

    return sqrt(ssq) * scale;
}

double uts_cblas_dasum(int n, const double* x, int incx) {
    if (n <= 0 || incx <= 0) return 0.0;

    const double* xp = x;

    double res = 0.0;

    for (int i = 0; i < n; i++) {
        res += fabs((*xp));
        xp += incx;
    }

    return res;
}

int uts_cblas_idamax(int n, const double* x, int incx) {
    if (n <= 0 || incx <= 0) return 0;

    const double* xp = x;

    int max_idx = 0;
    double max_val = -1.0;

    for (int i = 0; i < n; i++) {
        double t = fabs((*xp));
        if (t > max_val) max_idx = i + 1, max_val = t;
        xp += incx;
    }

    return max_idx;
}
