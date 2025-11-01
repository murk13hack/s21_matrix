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

/**
 * @brief Calculate one element of algebraic complements matrix
 * @param A Pointer to the source matrix
 * @param row Row index
 * @param col Column index
 * @param result Pointer to store the result value
 * @return Error code
 */
int s21_calc_complement_element(const matrix_t* A, int row, int col,
                                double* result);

/**
 * @brief Perform matrix multiplication for one element
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @param row Row index in result matrix
 * @param col Column index in result matrix
 * @return Calculated value
 */
double s21_mult_matrix_element(const matrix_t* A, const matrix_t* B, int row,
                                int col);

/**
 * @brief Calculate inverse matrix step by step
 * @param A Pointer to the source matrix
 * @param det Determinant value
 * @param result Pointer to store the result
 * @return Error code
 */
int s21_calculate_inverse_step(const matrix_t* A, double det,
                                matrix_t* result);

/**
 * @brief Validate input matrices for binary operations
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix (can be NULL for unary operations)
 * @param result Pointer to result matrix
 * @return Error code: 0 (OK) or error code
 */
int s21_validate_input_matrices(const matrix_t* A, const matrix_t* B,
                                 const matrix_t* result);

/**
 * @brief Apply binary operation element-wise to two matrices
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @param result Pointer to result matrix
 * @param operation Function pointer to operation (add/sub)
 * @return Error code
 */
int s21_apply_binary_operation(const matrix_t* A, const matrix_t* B,
                                matrix_t* result, double (*operation)(double, double));

/**
 * @brief Helper functions for binary operations (internal use)
 */
double s21_add_helper(double a, double b);
double s21_sub_helper(double a, double b);

#endif  // S21_MATRIX_HELPERS_H
