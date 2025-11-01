#include "../include/s21_matrix.h"
#include <math.h>
#include <stdlib.h>
#include "helpers/s21_matrix_helpers.h"

/**
 * @brief Create an m by n matrix
 * @param rows Number of rows in the matrix
 * @param columns Number of columns in the matrix
 * @param result Pointer to the initialized matrix
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_create_matrix(int rows, int columns, matrix_t* result) {
  if (result == NULL || rows <= 0 || columns <= 0) {
    return S21_ERROR_INCORRECT_MATRIX;
  }

  result->rows = rows;
  result->columns = columns;
  result->matrix = (double**)calloc(rows, sizeof(double*));

  if (result->matrix == NULL) {
    return S21_ERROR_CALCULATION;
  }

  for (int i = 0; i < rows; i++) {
    result->matrix[i] = (double*)calloc(columns, sizeof(double));
    if (result->matrix[i] == NULL) {
      // Clean up previously allocated memory
      for (int j = 0; j < i; j++) {
        free(result->matrix[j]);
      }
      free(result->matrix);
      result->matrix = NULL;
      return S21_ERROR_CALCULATION;
    }
  }

  return S21_OK;
}

/**
 * @brief Delete the matrix
 * @param A Pointer to the matrix to be deleted
 */
void s21_remove_matrix(matrix_t* A) {
  if (A != NULL && A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

/**
 * @brief Comparing two matrices
 * @param A Pointer to the matrix
 * @param B Pointer to the matrix
 * @return Error code: `1` (SUCCESS), `0` (FAILURE)
 */
int s21_eq_matrix(const matrix_t* A, const matrix_t* B) {
  if (A == NULL || B == NULL || A->matrix == NULL || B->matrix == NULL) {
    return FAILURE;
  }

  if (A->rows != B->rows || A->columns != B->columns) {
    return FAILURE;
  }

  const double EPSILON = 1e-6;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPSILON) {
        return FAILURE;
      }
    }
  }

  return SUCCESS;
}

/**
 * @brief Adding two matrices
 * @param A Pointer to the matrix
 * @param B Pointer to the matrix
 * @param result Pointer to summation result
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int status = S21_ERROR_INCORRECT_MATRIX;

  if (A != NULL && B != NULL && result != NULL) {
    if (s21_matrix_is_valid(A) && s21_matrix_is_valid(B)) {
      if (s21_matrices_same_size(A, B)) {
        status = s21_create_matrix(A->rows, A->columns, result);

        if (status == S21_OK) {
          for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
              result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
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

/**
 * @brief Multiplying a matrix by a number
 * @param A Pointer to the matrix
 * @param number Number to multiply by
 * @param result Pointer to result of the multiplication
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2`
 * (S21_ERROR_CALCULATION)
 */
int s21_mult_number(matrix_t* A, double number, matrix_t* result) {
  int status = S21_ERROR_INCORRECT_MATRIX;

  if (A != NULL && result != NULL) {
    if (s21_matrix_is_valid(A)) {
      status = s21_create_matrix(A->rows, A->columns, result);

      if (status == S21_OK) {
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < A->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] * number;
          }
        }
      }
    }
  }

  return status;
}

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
            sign = (i % 2 == 0) ? 1 : -1;

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
