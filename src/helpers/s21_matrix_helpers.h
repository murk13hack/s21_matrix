/**
 * @file s21_matrix_helpers.h
 * @brief Helper functions for matrix operations
 */

#ifndef S21_MATRIX_HELPERS_H
#define S21_MATRIX_HELPERS_H

#include "../include/s21_matrix.h"

/**
 * @brief Check if matrix is valid (not NULL, has valid dimensions, matrix
 * pointer exists)
 * @param A Pointer to the matrix
 * @return 1 if valid, 0 otherwise
 */
int s21_matrix_is_valid(const matrix_t* A);

/**
 * @brief Check if matrices have compatible dimensions for addition/subtraction
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @return 1 if compatible, 0 otherwise
 */
int s21_matrices_same_size(const matrix_t* A, const matrix_t* B);

/**
 * @brief Check if matrices have compatible dimensions for multiplication
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @return 1 if compatible, 0 otherwise
 */
int s21_matrices_mult_compatible(const matrix_t* A, const matrix_t* B);

/**
 * @brief Check if matrix is square
 * @param A Pointer to the matrix
 * @return 1 if square, 0 otherwise
 */
int s21_matrix_is_square(const matrix_t* A);

/**
 * @brief Create a minor matrix by removing row and column
 * @param A Pointer to the source matrix
 * @param row Row index to remove (0-based)
 * @param col Column index to remove (0-based)
 * @param result Pointer to the minor matrix result
 * @return Error code
 */
int s21_create_minor(const matrix_t* A, int row, int col, matrix_t* result);

/**
 * @brief Calculate determinant recursively using cofactor expansion
 * @param A Pointer to the matrix
 * @param result Pointer to store the determinant value
 * @return Error code
 */
int s21_determinant_recursive(const matrix_t* A, double* result);

#endif  // S21_MATRIX_HELPERS_H
