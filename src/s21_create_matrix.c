#include "../include/s21_matrix.h"
#include <stdlib.h>
#include <stddef.h>

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
