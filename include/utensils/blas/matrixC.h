#ifndef MATRIXC_H
#define MATRIXC_H

typedef struct {
    int rows;
    int cols;
    int size;
    double* data;
    int* row_order;
} MatrixC;

double dot(const double* a, const double* b, int size);
MatrixC* matrix_create(int rows, int cols);
void matrix_destroy(MatrixC* A);
MatrixC* matrix_copy(const MatrixC* A);
MatrixC* matrix_add(const MatrixC* A, const MatrixC* B);
MatrixC* matrix_mul(const MatrixC* A, const MatrixC* B);
MatrixC* matrix_transpose(const MatrixC* A);
void matrix_identity(MatrixC*A);
void matrix_print(const MatrixC* A, const char* name);

#endif