#include "../include/s21_matrix.h"
#include <math.h>
#include <stdlib.h>

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
