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
  int status = s21_validate_input_matrices(A, NULL, result);

  if (status != S21_OK) {
    return status;
  }

  if (!s21_matrix_is_square(A) || A->rows <= 1) {
    return S21_ERROR_CALCULATION;
  }

  status = s21_create_matrix(A->rows, A->columns, result);

  if (status != S21_OK) {
    return status;
  }

  int i = 0;
  int j = 0;
  int calc_status = S21_OK;

  while (i < A->rows && calc_status == S21_OK) {
    j = 0;
    while (j < A->columns && calc_status == S21_OK) {
      double element = 0.0;
      calc_status = s21_calc_complement_element(A, i, j, &element);

      if (calc_status == S21_OK) {
        result->matrix[i][j] = element;
      }
      j++;
    }
    i++;
  }

  if (calc_status != S21_OK) {
    s21_remove_matrix(result);
    status = calc_status;
  }

  return status;
}
