#include "matrixC.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EPS 1e-9

// ==================== 核心：LU 分解（带列主元） ====================
// 输入：A（方阵，会被拷贝到 U 中，原矩阵保持不变）
// 输出：L, U, P 满足 P * A = L * U

int lu_decompose(MatrixC* A, MatrixC** L_out, MatrixC** U_out, MatrixC** P_out) {
    int n = A->rows;
    if (A->cols != n) {
        printf("ERROR: LU decomposition is only for square matrices.\n");
        return 0;
    }

    // 1. 分配 L, U, P 矩阵
    MatrixC* L = matrix_create(n, n);
    MatrixC* U = matrix_copy(A);
    MatrixC* P = matrix_create(n, n);
    matrix_identity(P);
    
    int *Urow = U->row_order;
    int *Lrow = L->row_order;
    int *Prow = P->row_order;

    // 2. LU 分解主循环
    for (int i = 0; i < n; i++) {
        // 2.1 列主元查找
        // matrix_print(P, "P");
        // matrix_print(L, "L");
        // matrix_print(U, "U");
        int pivot = i;
        double max_val = fabs(U->data[Urow[i] * U->cols + i]);
        for (int j = i + 1; j < n; j++) {
            double val = fabs(U->data[Urow[j] * U->cols + i]);
            if (val > max_val) {
                max_val = val;
                pivot = j;
            }
        }
        if (max_val < EPS) {
            printf("WARNING: the matrix is bizarre, cannot do decomposition.\n");
            matrix_destroy(L);
            matrix_destroy(U);
            matrix_destroy(P);
            return 0;
        }

        // 3.2 交换行（交换指针，O(1)！）
        if (pivot != i) {
            int tmp = Urow[i];
            Urow[i] = Urow[pivot];
            Urow[pivot] = tmp; 

            Lrow[i] = Lrow[pivot];
            Lrow[pivot] = tmp;

            Prow[i] = Prow[pivot];
            Prow[pivot] = tmp;
        }

        // 3.3 消元
        for (int j = i + 1; j < n; j++) {
            double ratio = U->data[Urow[j] * U->cols + i] / U->data[Urow[i] * U->cols + i];
            L->data[Lrow[j] * L->cols + i] = ratio;
            for (int k = i; k < n; k++) {
                U->data[Urow[j] * U->cols + k] -= ratio * U->data[Urow[i] * U->cols + k];
            }
        }
    }
    for (int i = 0; i < L->rows; i++) L->data[Lrow[i] * L->cols + i] = 1.0;
    *L_out = L;
    *U_out = U;
    *P_out = P;
    return 1;
}

MatrixC* ux_solver(MatrixC *U, MatrixC* b) {
    if (!U || !b) return 0;
    MatrixC* x = matrix_create(U->cols, 1);
    MatrixC* current_line = matrix_create(1, U->cols);
    int* row = U->row_order;
    for (int i = b->size - 1; i >= 0; i--) {
        double sub_val = dot(&U->data[row[i] * U->cols], x->data, U->cols);
        x->data[i] = (b->data[i] - sub_val) / U->data[row[i] * U->cols + i];
    }
    matrix_destroy(current_line);
    return x;
}

MatrixC* lx_solver(MatrixC *L, MatrixC* b) {
    if (!L || !b) return 0;
    MatrixC* x = matrix_create(L->cols, 1);
    MatrixC* current_line = matrix_create(1, L->cols);
    int* row = L->row_order;
    for (int i = 0; i < b->size; i++) {
        double sub_val = dot(&L->data[row[i] * L->cols], x->data, L->cols);
        x->data[i] = (b->data[i] - sub_val) / L->data[row[i] * L->cols + i];
    }
    matrix_destroy(current_line);
    return x;
}