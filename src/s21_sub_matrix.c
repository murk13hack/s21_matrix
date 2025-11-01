#include <stddef.h>
#include "../include/s21_matrix.h"
#include "helpers/s21_matrix_helpers.h"

/**
 * @brief Subtraction two matrices
 * @param A Pointer to the matrix
 * @param B Pointer to the matrix
 * @param result Pointer to result of the subtraction
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int status = S21_ERROR_INCORRECT_MATRIX;

  if (A != NULL && B != NULL && result != NULL) {
    if (s21_matrix_is_valid(A) && s21_matrix_is_valid(B)) {
      if (s21_matrices_same_size(A, B)) {
        status = s21_create_matrix(A->rows, A->columns, result);

        if (status == S21_OK) {
          for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
              result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
            }
          }
        }
      } else {
        status = S21_ERROR_CALCULATION;
      }
    }
  }

  return status;
}
