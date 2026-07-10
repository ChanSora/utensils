#include <stdio.h>
#include <stdlib.h>

#include <utensils/blas.h>

void verify_lu(MatrixC* A, MatrixC* L, MatrixC* U, MatrixC* P) {
    int n = A->rows;
    MatrixC* PA = matrix_create(n, n);
    MatrixC* LU = matrix_create(n, n);

    PA = matrix_mul(P, A);

    LU = matrix_mul(L, U);

    matrix_print(PA, "PA");
    matrix_print(LU, "LU");

    matrix_destroy(PA);
    matrix_destroy(LU);
}

int main() {
    int n;
    printf("enter dim of n: ");
    scanf("%d", &n);

    MatrixC* A = matrix_create(n, n);
    printf("enter %d x %d matrix:\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%lf", &A->data[i * n + j]);
        }
    }

    MatrixC *L, *U, *P;
    if (!lu_decompose(A, &L, &U, &P)) {
        printf("LU composition failed.\n");
        matrix_destroy(A);
        return 1;
    }

    MatrixC* b = matrix_create(n, 1);
    printf("input %d * 1 vector:\n", n);
    for (int i = 0; i < n; i++) scanf("%lf", &b->data[i]);

    matrix_print(P, "P");
    matrix_print(L, "L");
    matrix_print(U, "U");

    verify_lu(A, L, U, P);

    MatrixC* Pb = matrix_mul(P, b);
    MatrixC* c = lx_solver(L, Pb);
    MatrixC* x = ux_solver(U, c);

    matrix_print(x, "x");

    printf("checking A * x = b...\n");

    MatrixC* Ax = matrix_mul(A, x);
    
    matrix_print(Ax, "Ax");
    matrix_print(b, "b");

    matrix_destroy(b);
    matrix_destroy(Pb);
    matrix_destroy(c);
    matrix_destroy(x);

    matrix_destroy(A);
    matrix_destroy(L);
    matrix_destroy(U);
    matrix_destroy(P);

    return 0;
}