#include "../include/s21_matrix.h"
#include "helpers/s21_matrix_helpers.h"
#include <stddef.h>

/**
 * @brief Multiplying a matrix by a matrix
 * @param A Pointer to the matrix
 * @param B Pointer to the matrix
 * @param result Pointer to result of the multiplication
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int status = S21_ERROR_INCORRECT_MATRIX;

  if (A != NULL && B != NULL && result != NULL) {
    if (s21_matrix_is_valid(A) && s21_matrix_is_valid(B)) {
      if (s21_matrices_mult_compatible(A, B)) {
        status = s21_create_matrix(A->rows, B->columns, result);

        if (status == S21_OK) {
          for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < B->columns; j++) {
              result->matrix[i][j] = 0.0;

              for (int k = 0; k < A->columns; k++) {
                result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
              }
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
