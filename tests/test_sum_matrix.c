#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/s21_matrix.h"

// Объявление функций логирования
void test_start(const char* test_name);
void test_end(const char* test_name);

// Helper function to check if two matrices are approximately equal
int matrices_equal(const matrix_t* A, const matrix_t* B, double tolerance) {
  if (A->rows != B->rows || A->columns != B->columns) return 0;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > tolerance) return 0;
    }
  }
  return 1;
}

// Test suite for s21_sum_matrix function
START_TEST(test_sum_matrix_valid) {
  test_start("test_sum_matrix_valid");
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 5.0;
  B.matrix[0][1] = 6.0;
  B.matrix[1][0] = 7.0;
  B.matrix[1][1] = 8.0;

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 6.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 8.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 10.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 12.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  test_end("test_sum_matrix_valid");
}
END_TEST

START_TEST(test_sum_matrix_different_sizes) {
  test_start("test_sum_matrix_different_sizes");
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 3, &B);

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_ERROR_CALCULATION);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  test_end("test_sum_matrix_different_sizes");
}
END_TEST

START_TEST(test_sum_matrix_null_A) {
  test_start("test_sum_matrix_null_A");
  matrix_t B, result;
  s21_create_matrix(2, 2, &B);

  int status = s21_sum_matrix(NULL, &B, &result);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);

  s21_remove_matrix(&B);
  test_end("test_sum_matrix_null_A");
}
END_TEST

START_TEST(test_sum_matrix_null_B) {
  test_start("test_sum_matrix_null_B");
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);

  int status = s21_sum_matrix(&A, NULL, &result);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);

  s21_remove_matrix(&A);
  test_end("test_sum_matrix_null_B");
}
END_TEST

START_TEST(test_sum_matrix_null_result) {
  test_start("test_sum_matrix_null_result");
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  int status = s21_sum_matrix(&A, &B, NULL);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  test_end("test_sum_matrix_null_result");
}
END_TEST

START_TEST(test_sum_matrix_with_zeros) {
  test_start("test_sum_matrix_with_zeros");
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 0.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = -1.0;

  B.matrix[0][0] = 0.0;
  B.matrix[0][1] = 1.0;
  B.matrix[1][0] = -1.0;
  B.matrix[1][1] = 0.0;

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 1.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], -1.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], -1.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  test_end("test_sum_matrix_with_zeros");
}
END_TEST

START_TEST(test_sum_matrix_negative_values) {
  test_start("test_sum_matrix_negative_values");
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = -1.0;
  A.matrix[0][1] = -2.0;
  A.matrix[1][0] = -3.0;
  A.matrix[1][1] = -4.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 0.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 0.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 0.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 0.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  test_end("test_sum_matrix_negative_values");
}
END_TEST

START_TEST(test_sum_matrix_single_element) {
  test_start("test_sum_matrix_single_element");
  matrix_t A, B, result;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);

  A.matrix[0][0] = 5.5;
  B.matrix[0][0] = 4.5;

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 10.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  test_end("test_sum_matrix_single_element");
}
END_TEST

Suite* sum_matrix_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("sum_matrix");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_sum_matrix_valid);
  tcase_add_test(tc_core, test_sum_matrix_different_sizes);
  tcase_add_test(tc_core, test_sum_matrix_null_A);
  tcase_add_test(tc_core, test_sum_matrix_null_B);
  tcase_add_test(tc_core, test_sum_matrix_null_result);
  tcase_add_test(tc_core, test_sum_matrix_with_zeros);
  tcase_add_test(tc_core, test_sum_matrix_negative_values);
  tcase_add_test(tc_core, test_sum_matrix_single_element);

  suite_add_tcase(s, tc_core);

  return s;
}