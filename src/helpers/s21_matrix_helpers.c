/**
 * @file s21_matrix_helpers.c
 * @brief Implementation of helper functions for matrix operations
 */

#include "s21_matrix_helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stddef.h>

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
          int i = 0;

          while (i < A->rows) {
            if (i != row) {
              int result_col = 0;
              int j = 0;

              while (j < A->columns) {
                if (j != col) {
                  result->matrix[result_row][result_col] = A->matrix[i][j];
                  result_col++;
                }
                j++;
              }
              result_row++;
            }
            i++;
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
        int col = 0;

        while (col < A->columns && calc_status == S21_OK) {
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
          col++;
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

int s21_calc_complement_element(const matrix_t* A, int row, int col,
                                double* result) {
  int status = S21_ERROR_INCORRECT_MATRIX;

  if (A == NULL || result == NULL) {
    return status;
  }

  matrix_t minor;
  double minor_det = 0.0;
  int sign = ((row + col) % 2 == 0) ? 1 : -1;

  status = s21_create_minor(A, row, col, &minor);

  if (status == S21_OK) {
    status = s21_determinant_recursive(&minor, &minor_det);
    s21_remove_matrix(&minor);

    if (status == S21_OK) {
      *result = sign * minor_det;
    }
  }

  return status;
}

double s21_mult_matrix_element(const matrix_t* A, const matrix_t* B, int row,
                                int col) {
  double result = 0.0;

  if (A == NULL || B == NULL) {
    return result;
  }

  if (row >= 0 && row < A->rows && col >= 0 && col < B->columns) {
    if (A->columns == B->rows) {
      int k = 0;
      while (k < A->columns) {
        result += A->matrix[row][k] * B->matrix[k][col];
        k++;
      }
    }
  }

  return result;
}

int s21_calculate_inverse_step(const matrix_t* A, double det,
                                matrix_t* result) {
  int status = S21_ERROR_CALCULATION;

  if (A == NULL || result == NULL) {
    return S21_ERROR_INCORRECT_MATRIX;
  }

  matrix_t complements;
  matrix_t transposed;

  status = s21_calc_complements((matrix_t*)A, &complements);

  if (status == S21_OK) {
    status = s21_transpose(&complements, &transposed);
    s21_remove_matrix(&complements);

    if (status == S21_OK) {
      status = s21_mult_number(&transposed, 1.0 / det, result);
      s21_remove_matrix(&transposed);
    }
  }

  return status;
}

int s21_validate_input_matrices(const matrix_t* A, const matrix_t* B,
                                 const matrix_t* result) {
  if (A == NULL || result == NULL) {
    return S21_ERROR_INCORRECT_MATRIX;
  }

  if (B != NULL && !s21_matrix_is_valid(B)) {
    return S21_ERROR_INCORRECT_MATRIX;
  }

  if (!s21_matrix_is_valid(A)) {
    return S21_ERROR_INCORRECT_MATRIX;
  }

  return S21_OK;
}

double s21_add_helper(double a, double b) { return a + b; }

double s21_sub_helper(double a, double b) { return a - b; }

int s21_apply_binary_operation(const matrix_t* A, const matrix_t* B,
                                matrix_t* result,
                                double (*operation)(double, double)) {
  int status = s21_validate_input_matrices(A, B, result);

  if (status != S21_OK) {
    return status;
  }

  if (!s21_matrices_same_size(A, B)) {
    return S21_ERROR_CALCULATION;
  }

  status = s21_create_matrix(A->rows, A->columns, result);

  if (status != S21_OK) {
    return status;
  }

  int i = 0;

  while (i < A->rows) {
    int j = 0;
    while (j < A->columns) {
      result->matrix[i][j] = operation(A->matrix[i][j], B->matrix[i][j]);
      j++;
    }
    i++;
  }

  return status;
}
