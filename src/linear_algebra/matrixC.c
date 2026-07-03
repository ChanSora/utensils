#include "matrixC.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EPS 1e-9

// ==================== 矩阵创建与销毁 ====================
MatrixC* matrix_create(int rows, int cols) {
    MatrixC* A = (MatrixC*)malloc(sizeof(MatrixC));
    A->rows = rows;
    A->cols = cols;
    A->size = rows * cols;
    A->data = (double*)malloc(rows * cols * sizeof(double));
    A->row_order = (int*)malloc(rows * sizeof(int));
    for (int i = 0; i < rows; i++) A->row_order[i] = i;
    return A;
}

void matrix_destroy(MatrixC* A) {
    if (!A) return;
    free(A->data);
    free(A->row_order);
    free(A);
}

// ==================== 矩阵复制 ====================
MatrixC* matrix_copy(const MatrixC* A) {
    MatrixC* B = matrix_create(A->rows, A->cols);

    memcpy(B->data, A->data, A->rows * A->cols * sizeof(double));
    memcpy(B->row_order, A->row_order, A->rows * sizeof(int));

    return B;
}

MatrixC* matrix_add(const MatrixC* A, const MatrixC* B) {
    if (A->rows != B->rows || A->cols != B->cols) {
        printf("ERROR: can't add the matrices.\n");
        return NULL;
    }

    MatrixC* res = matrix_create(A->rows, A->cols);
    
    int cols = A->cols;
    for (int i = 0; i < A->rows; i++) {
        int* ordA = A->row_order;
        int* ordB = B->row_order;
        double* rowA = &A->data[ordA[i] * cols];
        double* rowB = &B->data[ordB[i] * cols];
        double* rowR = &res->data[i * cols];
        for (int j = 0; j < cols; j++) {
            rowR[j] = rowA[j] + rowB[j];
        }
    }

    return res;
}

double dot(const double* a, const double* b, int size) {
    double res = 0;
    for (int i = 0; i < size; i++) res += a[i] * b[i];
    return res;
}

MatrixC* matrix_mul(const MatrixC* A, const MatrixC* B) {
    if (A->cols != B->rows) {
        printf("ERROR: can't multiply the matrices.\n");
        return NULL;
    }

    MatrixC* res = matrix_create(A->rows, B->cols);
    MatrixC* B_T = matrix_transpose(B);

    for (int i = 0; i < A->rows; i++) {
        int* ordA = A->row_order;
        double* rowA = &A->data[ordA[i] * A->cols];
        for (int j = 0; j < B_T->rows; j++) {
            int* ordB_T = B_T->row_order; 
            double* rowB_T = &B_T->data[ordB_T[j] * B_T->cols];
            res->data[i * B_T->rows + j] = dot(rowA, rowB_T, A->cols);
        }
    }

    matrix_destroy(B_T);

    return res;
}

MatrixC* matrix_transpose(const MatrixC* A) {
    int rows = A->rows, cols = A->cols;
    MatrixC* res = matrix_create(cols, rows);
    
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            res->data[j * rows + i] = A->data[A->row_order[i] * cols + j];
    
    return res;
}

// ==================== 初始化单位矩阵 ====================
void matrix_identity(MatrixC* A) {
    for (int i = 0; i < A->rows; i++) 
        for (int j = 0; j < A->cols; j++) 
            A->data[i * A->cols + j] = (i == j) ? 1.0 : 0.0;
}

// ==================== 打印矩阵 ====================
void matrix_print(const MatrixC* A, const char* name) {
    printf("%s:\n", name);
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->cols; j++) {
            printf("%8.4f ", A->data[A->row_order[i] * A->cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

// 把数据重排列为正确顺序，而不是通过row_order访问
void matrix_flatten(MatrixC* A) {
    if (!A) return;
    double* tmp = (double*)malloc(A->size * sizeof(double));
    for (int i = 0; i < A->rows; i++) {
        memcpy(&tmp[i * A->cols], &A->data[A->row_order[i] * A->cols], A->cols * sizeof(double));
        A->row_order[i] = i;
    }
    memcpy(A->data, tmp, A->size * sizeof(double));
    free(tmp);
}
