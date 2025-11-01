#include "../include/s21_matrix.h"
#include "helpers/s21_matrix_helpers.h"
#include <stddef.h>

/**
 * @brief Minor matrices and the matrix of algebraic complements
 * @param A Pointer to the matrix
 * @param result Pointer to algebraic complements
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_calc_complements(matrix_t* A, matrix_t* result) {
  int status = S21_ERROR_INCORRECT_MATRIX;

  if (A != NULL && result != NULL) {
    if (s21_matrix_is_valid(A)) {
      if (s21_matrix_is_square(A) && A->rows > 1) {
        status = s21_create_matrix(A->rows, A->columns, result);

        if (status == S21_OK) {
          int sign = 1;
          int calc_status = S21_OK;

          for (int i = 0; i < A->rows && calc_status == S21_OK; i++) {
            for (int j = 0; j < A->columns && calc_status == S21_OK; j++) {
              matrix_t minor;
              double minor_det = 0.0;

              sign = ((i + j) % 2 == 0) ? 1 : -1;

              calc_status = s21_create_minor(A, i, j, &minor);

              if (calc_status == S21_OK) {
                calc_status = s21_determinant_recursive(&minor, &minor_det);
                s21_remove_matrix(&minor);

                if (calc_status == S21_OK) {
                  result->matrix[i][j] = sign * minor_det;
                }
              }
            }
          }

          if (calc_status != S21_OK) {
            status = calc_status;
            s21_remove_matrix(result);
          }
        }
      } else {
        status = S21_ERROR_CALCULATION;
      }
    }
  }

  return status;
}
