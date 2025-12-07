#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/s21_matrix.h"

// Объявление функций логирования
void test_start(const char* test_name);
void test_end(const char* test_name);

// Test suite for s21_transpose function
START_TEST(test_transpose_valid) {
  test_start("test_transpose_valid");
  matrix_t A, result;
  s21_create_matrix(2, 3, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;

  int status = s21_transpose(&A, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 2);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 4.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 2.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 5.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[2][0], 3.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[2][1], 6.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  test_end("test_transpose_valid");
}
END_TEST

START_TEST(test_transpose_square) {
  test_start("test_transpose_square");
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int status = s21_transpose(&A, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 3.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 2.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 4.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  test_end("test_transpose_square");
}
END_TEST

START_TEST(test_transpose_single_element) {
  test_start("test_transpose_single_element");
  matrix_t A, result;
  s21_create_matrix(1, 1, &A);

  A.matrix[0][0] = 5.5;

  int status = s21_transpose(&A, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 5.5, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  test_end("test_transpose_single_element");
}
END_TEST

START_TEST(test_transpose_null_A) {
  test_start("test_transpose_null_A");
  matrix_t result;

  int status = s21_transpose(NULL, &result);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
  test_end("test_transpose_null_A");
}
END_TEST

START_TEST(test_transpose_null_result) {
  test_start("test_transpose_null_result");
  matrix_t A;
  s21_create_matrix(2, 2, &A);

  int status = s21_transpose(&A, NULL);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);

  s21_remove_matrix(&A);
  test_end("test_transpose_null_result");
}
END_TEST

START_TEST(test_transpose_row_vector) {
  test_start("test_transpose_row_vector");
  matrix_t A, result;
  s21_create_matrix(1, 3, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;

  int status = s21_transpose(&A, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 1);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 2.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[2][0], 3.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  test_end("test_transpose_row_vector");
}
END_TEST

Suite* transpose_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("transpose");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_transpose_valid);
  tcase_add_test(tc_core, test_transpose_square);
  tcase_add_test(tc_core, test_transpose_single_element);
  tcase_add_test(tc_core, test_transpose_null_A);
  tcase_add_test(tc_core, test_transpose_null_result);
  tcase_add_test(tc_core, test_transpose_row_vector);

  suite_add_tcase(s, tc_core);

  return s;
}