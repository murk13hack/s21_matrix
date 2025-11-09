#include <stddef.h>
#include "../include/s21_matrix.h"
#include "helpers/s21_matrix_helpers.h"

/**
 * @brief Minor matrices and the matrix of algebraic complements
 * @param A Pointer to the matrix
 * @param result Pointer to algebraic complements
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_calc_complements(matrix_t* A, matrix_t* result) {
  if (!s21_init_matrix(result)) {
    return S21_ERROR_INCORRECT_MATRIX;
  }

  int status = s21_validate_single_matrix(A);
  if (status != S21_OK)
    return S21_ERROR_INCORRECT_MATRIX;

  if (!s21_matrix_is_square(A) || A->rows <= 1) {
    return S21_ERROR_CALCULATION;
  }

  status = s21_create_matrix(A->rows, A->columns, result);
  if (status != S21_OK)
    return status;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      double element = 0.0;
      status = s21_calc_complement_element(A, i, j, &element);

      if (status != S21_OK) {
        s21_remove_matrix(result);
        return status;
      }

      result->matrix[i][j] = element;
    }
  }
  return S21_OK;
}
