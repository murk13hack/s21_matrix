#include <stddef.h>
#include "../include/s21_matrix.h"
#include "helpers/s21_matrix_helpers.h"

/**
 * @brief Adding two matrices
 * @param A Pointer to the matrix
 * @param B Pointer to the matrix
 * @param result Pointer to summation result
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  
  if (!s21_init_matrix(result)) {
    return S21_ERROR_INCORRECT_MATRIX;
  }

  int status = s21_validate_two_matrices(A, B);

  if (status != S21_OK)
    return status;

  if (!s21_matrices_same_size(A, B)) {
    return S21_ERROR_CALCULATION;
  }

  return s21_apply_binary_operation(A, B, result, s21_add_helper);
}
