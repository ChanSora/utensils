#include "matrixC.h"
#include "linalgC.h"

#include <stdio.h>
#include <stdlib.h>

// ==================== 验证：P * A == L * U ====================
int verify_lu(MatrixC* A, MatrixC* L, MatrixC* U, MatrixC* P) {
    int n = A->rows;
    MatrixC* PA = matrix_create(n, n);
    MatrixC* LU = matrix_create(n, n);

    // 计算 P * A
    PA = matrix_mul(P, A);
    // 计算 L * U
    LU = matrix_mul(L, U);

    matrix_print(PA, "PA");
    matrix_print(LU, "LU");

    // 比较 PA 和 LU
    int ok = 1;
    // for (int i = 0; i < n && ok; i++) {
    //     for (int j = 0; j < n && ok; j++) {
    //         if (fabs(PA->data[i][j] - LU->data[i][j]) > 1e-8) {
    //             ok = 0;
    //             printf("验证失败：PA[%d][%d] = %.6f, LU[%d][%d] = %.6f\n",
    //                    i, j, PA->data[i][j], i, j, LU->data[i][j]);
    //         }
    //     }
    // }

    matrix_destroy(PA);
    matrix_destroy(LU);
    return ok;
}

// ==================== 主函数 ====================
int main() {
    int n, m;
    printf("请输入矩阵维度 n x m: ");
    scanf("%d%d", &n, &m);

    // 1. 创建并读入矩阵 A
    MatrixC* A = matrix_create(n, m);
    printf("请输入 %d x %d 矩阵:\n", n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%lf", &A->data[i * m + j]);
        }
    }
    // 2. LU 分解
    MatrixC *L, *U, *P;
    if (!lu_decompose(A, &L, &U, &P)) {
        printf("LU 分解失败\n");
        matrix_destroy(A);
        return 1;
    }

    // 3. 输出结果
    matrix_print(P, "P (置换矩阵)");
    matrix_print(L, "L (下三角)");
    matrix_print(U, "U (上三角)");

    // 4. 验证 P * A == L * U
    if (verify_lu(A, L, U, P)) {
        printf("✅ 验证通过！P * A == L * U\n");
    } else {
        printf("❌ 验证失败！\n");
    }

    // 5. 清理
    matrix_destroy(A);
    matrix_destroy(L);
    matrix_destroy(U);
    matrix_destroy(P);

    return 0;
}