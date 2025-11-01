/**
 * @file s21_matrix_helpers.c
 * @brief Implementation of helper functions for matrix operations
 */

#include "s21_matrix_helpers.h"
#include <math.h>
#include <stdlib.h>

int s21_matrix_is_valid(const matrix_t* A) {
  int result = 0;

  if (A != NULL && A->matrix != NULL) {
    if (A->rows > 0 && A->columns > 0) {
      result = 1;
    }
  }

  return result;
}

int s21_matrices_same_size(const matrix_t* A, const matrix_t* B) {
  int result = 0;

  if (s21_matrix_is_valid(A) && s21_matrix_is_valid(B)) {
    if (A->rows == B->rows && A->columns == B->columns) {
      result = 1;
    }
  }

  return result;
}

int s21_matrices_mult_compatible(const matrix_t* A, const matrix_t* B) {
  int result = 0;

  if (s21_matrix_is_valid(A) && s21_matrix_is_valid(B)) {
    if (A->columns == B->rows) {
      result = 1;
    }
  }

  return result;
}

int s21_matrix_is_square(const matrix_t* A) {
  int result = 0;

  if (s21_matrix_is_valid(A)) {
    if (A->rows == A->columns) {
      result = 1;
    }
  }

  return result;
}

int s21_create_minor(const matrix_t* A, int row, int col, matrix_t* result) {
  int status = S21_ERROR_INCORRECT_MATRIX;

  if (s21_matrix_is_valid(A) && result != NULL) {
    if (row >= 0 && row < A->rows && col >= 0 && col < A->columns) {
      int minor_rows = A->rows - 1;
      int minor_cols = A->columns - 1;

      if (minor_rows > 0 && minor_cols > 0) {
        status = s21_create_matrix(minor_rows, minor_cols, result);

        if (status == S21_OK) {
          int result_row = 0;
          int result_col = 0;

          for (int i = 0; i < A->rows; i++) {
            if (i != row) {
              result_col = 0;
              for (int j = 0; j < A->columns; j++) {
                if (j != col) {
                  result->matrix[result_row][result_col] = A->matrix[i][j];
                  result_col++;
                }
              }
              result_row++;
            }
          }
        }
      }
    }
  }

  return status;
}

int s21_determinant_recursive(const matrix_t* A, double* result) {
  int status = S21_ERROR_INCORRECT_MATRIX;

  if (s21_matrix_is_valid(A) && result != NULL) {
    if (s21_matrix_is_square(A)) {
      if (A->rows == 1) {
        *result = A->matrix[0][0];
        status = S21_OK;
      } else if (A->rows == 2) {
        *result = A->matrix[0][0] * A->matrix[1][1] -
                  A->matrix[0][1] * A->matrix[1][0];
        status = S21_OK;
      } else {
        double determinant = 0.0;
        int sign = 1;
        int calc_status = S21_OK;

        for (int col = 0; col < A->columns && calc_status == S21_OK; col++) {
          matrix_t minor;
          double minor_det = 0.0;

          calc_status = s21_create_minor(A, 0, col, &minor);

          if (calc_status == S21_OK) {
            calc_status = s21_determinant_recursive(&minor, &minor_det);
            s21_remove_matrix(&minor);

            if (calc_status == S21_OK) {
              determinant += sign * A->matrix[0][col] * minor_det;
              sign = -sign;
            }
          }
        }

        if (calc_status == S21_OK) {
          *result = determinant;
          status = S21_OK;
        } else {
          status = calc_status;
        }
      }
    } else {
      status = S21_ERROR_CALCULATION;
    }
  }

  return status;
}
