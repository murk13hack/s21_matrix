#include <stddef.h>
#include "../include/s21_matrix.h"
#include "helpers/s21_matrix_helpers.h"

/**
 * @brief Matrix transposition
 * @param A Pointer to the matrix
 * @param result Pointer to result of the transposition
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_transpose(matrix_t* A, matrix_t* result) {
  int status = s21_validate_input_matrices(A, NULL, result);

  if (status != S21_OK) {
    return status;
  }

  status = s21_create_matrix(A->columns, A->rows, result);

  if (status != S21_OK) {
    return status;
  }

  int i = 0;

  while (i < A->rows) {
    int j = 0;
    while (j < A->columns) {
      result->matrix[j][i] = A->matrix[i][j];
      j++;
    }
    i++;
  }

  return status;
}
