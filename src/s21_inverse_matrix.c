#include <math.h>
#include <stddef.h>
#include "../include/s21_matrix.h"
#include "helpers/s21_matrix_helpers.h"

/**
 * @brief Finding the inverse matrix
 * @param A Pointer to the matrix
 * @param result Pointer to inverse matrix
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_inverse_matrix(matrix_t* A, matrix_t* result) {
  if (!s21_init_matrix(result)) {
    return S21_ERROR_INCORRECT_MATRIX;
  }

  int status = s21_validate_single_matrix(A);

  if (status != S21_OK) {
    return status;
  }

  if (!s21_matrix_is_square(A)) {
    return S21_ERROR_CALCULATION;
  }

  double det = 0.0;
  status = s21_determinant(A, &det);

  if (status != S21_OK) {
    return status;
  }

  if (fabs(det) < DETERMINANT_EPSILON) {
    return S21_ERROR_CALCULATION;
  }

  status = s21_calculate_inverse_step(A, det, result);

  return status;
}
