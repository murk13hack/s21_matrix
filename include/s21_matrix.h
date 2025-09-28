/**
 * @file s21_matrix.h
 * @brief Matrix arithmetic library implementation
 */

#ifndef S21_MATRIX_H
#define S21_MATRIX_H

/** @brief Structure representing the matrix
 */
typedef struct matrix_struct {
    double** matrix;
    int rows;
    int columns;
} matrix_t;

/*======================================================================
    STATUS CODE DEFINITIONS
======================================================================*/

/* The resulting opcode (except matrix comparison) */
#define S21_OK 0
#define S21_ERROR_INCORRECT_MATRIX 1
#define S21_ERROR_CALCULATION 2

/* The resulting code for the operation: matrix comparison */
#define SUCCESS 1
#define FAILURE 0

#endif

/*======================================================================
OPERATIONS ON MATRICES
======================================================================*/

/**
 * @brief Create an m by n matrix
 * @param rows Number of rows in the matrix
 * @param columns Number of columns in the matrix
 * @param result Pointer to the initialized matrix
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2` (S21_ERROR_CALCULATION)
 * @author murk13hack
 * @date 28.09.25
 */
int s21_create_matrix(int rows, int columns, matrix_t *result);

/**
 * @brief Delete the matrix
 * @param A Pointer to the matrix to be deleted
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2` (S21_ERROR_CALCULATION)
 * @author murk13hack
 * @date 28.09.25
 */
void s21_remove_matrix(matrix_t *A);

/**
 * @brief Comparing two matrices
 * @param A Pointer to the matrix
 * @param B Pointer to the matrix
 * @return Error code: `1` (SUCCESS), `0` (FAILURE)
 * @author murk13hack
 * @date 28.09.25
 */
int s21_eq_matrix(matrix_t *A, matrix_t *B);

/**
 * @brief Adding two matrices
 * @param A Pointer to the matrix
 * @param B Pointer to the matrix
 * @param result Pointer to summation result
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2` (S21_ERROR_CALCULATION)
 * @author murk13hack
 * @date 28.09.25
 */
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/**
 * @brief Subtraction two matrices
 * @param A Pointer to the matrix
 * @param B Pointer to the matrix
 * @param result Pointer to result of the subtraction
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2` (S21_ERROR_CALCULATION)
 * @author murk13hack
 * @date 28.09.25
 */
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/**
 * @brief Multiplying a matrix by a number
 * @param A Pointer to the matrix
 * @param B Number
 * @param result Pointer to result of the multiplication
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2` (S21_ERROR_CALCULATION)
 * @author murk13hack
 * @date 28.09.25
 */
int s21_mult_number(matrix_t *A, double number, matrix_t *result);

/**
 * @brief Multiplying a matrix by a matrix
 * @param A Pointer to the matrix
 * @param B Pointer to the matrix
 * @param result Pointer to result of the multiplication
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2` (S21_ERROR_CALCULATION)
 * @author murk13hack
 * @date 28.09.25
 */
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/**
 * @brief Matrix transposition
 * @param A Pointer to the matrix
 * @param result Pointer to result of the transposition
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2` (S21_ERROR_CALCULATION)
 * @author murk13hack
 * @date 28.09.25
 */
int s21_transpose(matrix_t *A, matrix_t *result);

/**
 * @brief Minor matrices and the matrix of algebraic complements
 * @param A Pointer to the matrix
 * @param result Pointer to algebraic complements
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2` (S21_ERROR_CALCULATION)
 * @author murk13hack
 * @date 28.09.25
 */
int s21_calc_complements(matrix_t *A, matrix_t *result);

/**
 * @brief The search for the determinant
 * @param A Pointer to the matrix
 * @param result Pointer to determinant
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2` (S21_ERROR_CALCULATION)
 * @author murk13hack
 * @date 28.09.25
 */
int s21_determinant(matrix_t *A, double *result);

/**
 * @brief Finding the inverse matrix
 * @param A Pointer to the matrix
 * @param result Pointer to inverse matrix
 * @return Error code: `0` (OK), `1` (S21_ERROR_INCORRECT_MATRIX), `2` (S21_ERROR_CALCULATION)
 * @author murk13hack
 * @date 28.09.25
 */
int s21_inverse_matrix(matrix_t *A, matrix_t *result);
