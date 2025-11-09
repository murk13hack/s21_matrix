#include <stddef.h>
#include "../include/s21_matrix.h"
#include "helpers/s21_matrix_helpers.h"

/**
 * @brief Multiplying a matrix by a matrix
 * @param A Pointer to the matrix
 * @param B Pointer to the matrix
 * @param result Pointer to result of the multiplication
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (!s21_init_matrix(result)) {
    return S21_ERROR_INCORRECT_MATRIX;
  }

  int status = s21_validate_two_matrices(A, B);

  if (status != S21_OK) {
    return status;
  }

  if (!s21_matrices_mult_compatible(A, B)) {
    return S21_ERROR_CALCULATION;
  }

  status = s21_create_matrix(A->rows, B->columns, result);

  if (status != S21_OK) {
    return status;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      result->matrix[i][j] = s21_mult_matrix_element(A, B, i, j);
    }
  }

  return status;
}
