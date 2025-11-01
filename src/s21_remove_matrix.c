#include "../include/s21_matrix.h"
#include <stdlib.h>
#include <stddef.h>

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
