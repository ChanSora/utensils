#include "matrixC.h"

int lu_decompose(MatrixC* A, MatrixC** L_out, MatrixC** U_out, MatrixC** P_out);
MatrixC* ux_solver(MatrixC *U, MatrixC* b);
MatrixC* lx_solver(MatrixC *L, MatrixC* b);