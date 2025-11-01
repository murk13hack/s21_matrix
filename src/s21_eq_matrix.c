#include <math.h>
#include <stddef.h>
#include "../include/s21_matrix.h"

/**
 * @brief Comparing two matrices
 * @param A Pointer to the matrix
 * @param B Pointer to the matrix
 * @return Error code: `1` (SUCCESS), `0` (FAILURE)
 */
int s21_eq_matrix(const matrix_t* A, const matrix_t* B) {
  if (A == NULL || B == NULL || A->matrix == NULL || B->matrix == NULL) {
    return FAILURE;
  }

  if (A->rows != B->rows || A->columns != B->columns) {
    return FAILURE;
  }

  const double EPSILON = 1e-6;
  int i = 0;
  int equal = 1;

  while (i < A->rows && equal) {
    int j = 0;
    while (j < A->columns && equal) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPSILON) {
        equal = 0;
      }
      j++;
    }
    i++;
  }

  return equal ? SUCCESS : FAILURE;
}
