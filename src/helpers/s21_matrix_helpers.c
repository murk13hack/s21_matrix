/**
 * @file s21_matrix_helpers.c
 * @brief Implementation of helper functions for matrix operations
 */

#include "s21_matrix_helpers.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

int s21_matrix_is_valid(const matrix_t* A) {
  if (A == NULL)
    return FAILURE;
  if (A->matrix == NULL || A->rows <= 0 || A->columns <= 0)
    return FAILURE;

  for (int i = 0; i < A->rows; i++) {
    if (A->matrix[i] == NULL)
      return FAILURE;
  }

  return SUCCESS;
}

int s21_matrices_same_size(const matrix_t* A, const matrix_t* B) {
  int result = FAILURE;

  if (s21_matrix_is_valid(A) && s21_matrix_is_valid(B)) {
    if (A->rows == B->rows && A->columns == B->columns) {
      result = SUCCESS;
    }
  }

  return result;
}

int s21_matrices_mult_compatible(const matrix_t* A, const matrix_t* B) {
  int result = FAILURE;

  if (s21_matrix_is_valid(A) && s21_matrix_is_valid(B)) {
    if (A->columns == B->rows) {
      result = SUCCESS;
    }
  }

  return result;
}

int s21_matrix_is_square(const matrix_t* A) {
  int result = FAILURE;

  if (s21_matrix_is_valid(A)) {
    if (A->rows == A->columns) {
      result = SUCCESS;
    }
  }

  return result;
}

int s21_create_minor(const matrix_t* A, int row, int col, matrix_t* result) {
  int minor_rows = A->rows - 1;
  int minor_cols = A->columns - 1;

  if (minor_rows <= 0 || minor_cols <= 0) {
    return s21_create_matrix(0, 0, result);
  }

  int status = s21_create_matrix(minor_rows, minor_cols, result);

  if (status != S21_OK) {
    return status;
  }

  for (int i = 0, res_i = 0; i < A->rows; i++) {
    if (i == row)
      continue;

    for (int j = 0, res_j = 0; j < A->columns; j++) {
      if (j == col)
        continue;
      result->matrix[res_i][res_j] = A->matrix[i][j];
      res_j++;
    }
    res_i++;
  }
  return status;
}

int s21_determinant_recursive(const matrix_t* A, double* result) {
  if (A->rows == 1) {
    *result = A->matrix[0][0];
    return S21_OK;
  }

  if (A->rows == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    return S21_OK;
  }

  double determinant = 0.0;
  int sign = 1;
  int status = S21_OK;

  for (int column = 0; column < A->columns && status == S21_OK; column++) {
    matrix_t minor = {0};
    double minor_determinant = 0.0;

    status = s21_create_minor(A, 0, column, &minor);
    if (status != S21_OK)
      break;

    status = s21_determinant_recursive(&minor, &minor_determinant);
    s21_remove_matrix(&minor);

    if (status == S21_OK) {
      determinant += sign * A->matrix[0][column] * minor_determinant;
      sign = -sign;
    }
  }

  if (status == S21_OK) {
    *result = determinant;
  }
  return status;
}

int s21_calc_complement_element(const matrix_t* A, int row, int col,
                                double* result) {
  if (!s21_matrix_is_valid(A) || result == NULL) {
    return S21_ERROR_INCORRECT_MATRIX;
  }

  if (!s21_matrix_is_square(A)) {
    return S21_ERROR_CALCULATION;
  }

  if (!hfh_within_bounds(A, row, col)) {
    return S21_ERROR_CALCULATION;
  }

  matrix_t minor = {0};
  double minor_det = 0.0;
  int sign = hfh_calculate_complement_sign(row, col);

  int status = s21_create_minor(A, row, col, &minor);
  if (status == S21_OK) {
    status = s21_determinant(&minor, &minor_det);
    s21_remove_matrix(&minor);
  }

  if (status == S21_OK) {
    *result = sign * minor_det;
  }

  return status;
}

double s21_mult_matrix_element(const matrix_t* A, const matrix_t* B, int row,
                               int col) {
  double result = 0.0;

  for (int k = 0; k < A->columns; k++) {
    result += A->matrix[row][k] * B->matrix[k][col];
  }

  return result;
}

int s21_calculate_inverse_step(matrix_t* A, double det, matrix_t* result) {
  if (!s21_matrix_is_valid(A) || result == NULL) {
    return S21_ERROR_INCORRECT_MATRIX;
  }

  if (!s21_matrix_is_square(A)) {
    return S21_ERROR_CALCULATION;
  }

  if (fabs(det) < DETERMINANT_EPSILON) {
    return S21_ERROR_CALCULATION;
  }

  matrix_t complements = {0};
  matrix_t transposed = {0};
  int status;

  status = s21_calc_complements(A, &complements);
  if (status != S21_OK)
    return status;

  status = s21_transpose(&complements, &transposed);
  s21_remove_matrix(&complements);

  if (status != S21_OK) {
    s21_remove_matrix(&transposed);
    return status;
  }

  status = s21_mult_number(&transposed, 1.0 / det, result);
  s21_remove_matrix(&transposed);

  return status;
}

int s21_validate_two_matrices(const matrix_t* A, const matrix_t* B,
                              const matrix_t* result) {
  if (A == NULL || B == NULL || result == NULL) {
    return S21_ERROR_INCORRECT_MATRIX;
  }
  if (!s21_matrix_is_valid(A) || !s21_matrix_is_valid(B)) {
    return S21_ERROR_INCORRECT_MATRIX;
  }
  return S21_OK;
}

int s21_validate_single_matrix(const matrix_t* A, const matrix_t* result) {
  if (A == NULL || result == NULL) {
    return S21_ERROR_INCORRECT_MATRIX;
  }
  if (!s21_matrix_is_valid(A)) {
    return S21_ERROR_INCORRECT_MATRIX;
  }
  return S21_OK;
}

double s21_add_helper(double a, double b) {
  return a + b;
}

double s21_sub_helper(double a, double b) {
  return a - b;
}

int s21_apply_binary_operation(const matrix_t* A, const matrix_t* B,
                               matrix_t* result,
                               double (*operation)(double, double)) {
  int status = s21_create_matrix(A->rows, A->columns, result);
  if (status != S21_OK)
    return status;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = operation(A->matrix[i][j], B->matrix[i][j]);
    }
  }

  return S21_OK;
}
//
//                                  ---helpers for helpers---
//
int hfh_within_bounds(const matrix_t* A, int row, int col) {
  return (A != NULL && row >= 0 && row < A->rows && col >= 0 &&
          col < A->columns);
}

int hfh_calculate_complement_sign(int row, int col) {
  return ((row + col) % 2 == 0) ? 1 : -1;
}