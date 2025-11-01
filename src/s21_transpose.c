#include "../include/s21_matrix.h"
#include "helpers/s21_matrix_helpers.h"
#include <stddef.h>

/**
 * @brief Matrix transposition
 * @param A Pointer to the matrix
 * @param result Pointer to result of the transposition
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_transpose(matrix_t* A, matrix_t* result) {
  int status = S21_ERROR_INCORRECT_MATRIX;

  if (A != NULL && result != NULL) {
    if (s21_matrix_is_valid(A)) {
      status = s21_create_matrix(A->columns, A->rows, result);

      if (status == S21_OK) {
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < A->columns; j++) {
            result->matrix[j][i] = A->matrix[i][j];
          }
        }
      }
    }
  }

  return status;
}
