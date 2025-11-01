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
  int status = S21_ERROR_INCORRECT_MATRIX;

  if (A != NULL && result != NULL) {
    if (s21_matrix_is_valid(A)) {
      if (s21_matrix_is_square(A)) {
        double det = 0.0;
        int det_status = s21_determinant(A, &det);

        if (det_status == S21_OK) {
          if (fabs(det) > 1e-6) {
            matrix_t complements;
            matrix_t transposed;
            int calc_status = S21_OK;

            calc_status = s21_calc_complements(A, &complements);

            if (calc_status == S21_OK) {
              calc_status = s21_transpose(&complements, &transposed);
              s21_remove_matrix(&complements);

              if (calc_status == S21_OK) {
                calc_status = s21_mult_number(&transposed, 1.0 / det, result);
                s21_remove_matrix(&transposed);
              }
            }

            status = calc_status;
          } else {
            status = S21_ERROR_CALCULATION;
          }
        } else {
          status = det_status;
        }
      } else {
        status = S21_ERROR_CALCULATION;
      }
    }
  }

  return status;
}
