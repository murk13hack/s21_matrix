#include <stddef.h>
#include "../include/s21_matrix.h"
#include "helpers/s21_matrix_helpers.h"

/**
 * @brief Multiplying a matrix by a number
 * @param A Pointer to the matrix
 * @param number Number to multiply by
 * @param result Pointer to result of the multiplication
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_mult_number(matrix_t* A, double number, matrix_t* result) {
  int status = s21_validate_single_matrix(A, result);

  if (status != S21_OK) {
    return status;
  }

  status = s21_create_matrix(A->rows, A->columns, result);

  if (status != S21_OK) {
    return status;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }

  return status;
}
