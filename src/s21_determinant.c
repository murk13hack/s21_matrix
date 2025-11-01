#include <stddef.h>
#include "../include/s21_matrix.h"
#include "helpers/s21_matrix_helpers.h"

/**
 * @brief The search for the determinant
 * @param A Pointer to the matrix
 * @param result Pointer to determinant
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_determinant(matrix_t* A, double* result) {
  int status = S21_ERROR_INCORRECT_MATRIX;

  if (A != NULL && result != NULL) {
    status = s21_determinant_recursive(A, result);
  }

  return status;
}
