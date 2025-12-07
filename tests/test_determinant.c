#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/s21_matrix.h"

// Объявление функций логирования
void test_start(const char* test_name);
void test_end(const char* test_name);

// Test suite for s21_determinant function
START_TEST(test_determinant_2x2) {
  test_start("test_determinant_2x2");
  matrix_t A;
  double result;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int status = s21_determinant(&A, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result, -2.0, 1e-7);

  s21_remove_matrix(&A);
  test_end("test_determinant_2x2");
}
END_TEST

START_TEST(test_determinant_3x3) {
  test_start("test_determinant_3x3");
  matrix_t A;
  double result;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;
  A.matrix[2][0] = 7.0;
  A.matrix[2][1] = 8.0;
  A.matrix[2][2] = 9.0;

  int status = s21_determinant(&A, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result, 0.0, 1e-7);

  s21_remove_matrix(&A);
  test_end("test_determinant_3x3");
}
END_TEST

START_TEST(test_determinant_1x1) {
  test_start("test_determinant_1x1");
  matrix_t A;
  double result;
  s21_create_matrix(1, 1, &A);

  A.matrix[0][0] = 5.0;

  int status = s21_determinant(&A, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result, 5.0, 1e-7);

  s21_remove_matrix(&A);
  test_end("test_determinant_1x1");
}
END_TEST

START_TEST(test_determinant_non_square) {
  test_start("test_determinant_non_square");
  matrix_t A;
  double result;
  s21_create_matrix(2, 3, &A);

  int status = s21_determinant(&A, &result);
  ck_assert_int_eq(status, S21_ERROR_CALCULATION);

  s21_remove_matrix(&A);
  test_end("test_determinant_non_square");
}
END_TEST

START_TEST(test_determinant_null_A) {
  test_start("test_determinant_null_A");
  double result;

  int status = s21_determinant(NULL, &result);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
  test_end("test_determinant_null_A");
}
END_TEST

START_TEST(test_determinant_null_result) {
  test_start("test_determinant_null_result");
  matrix_t A;
  s21_create_matrix(2, 2, &A);

  int status = s21_determinant(&A, NULL);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);

  s21_remove_matrix(&A);
  test_end("test_determinant_null_result");
}
END_TEST

START_TEST(test_determinant_zero) {
  test_start("test_determinant_zero");
  matrix_t A;
  double result;
  s21_create_matrix(3, 3, &A);

  // Matrix with linearly dependent rows
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 6.0;
  A.matrix[2][0] = 3.0;
  A.matrix[2][1] = 6.0;
  A.matrix[2][2] = 9.0;

  int status = s21_determinant(&A, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result, 0.0, 1e-7);

  s21_remove_matrix(&A);
  test_end("test_determinant_zero");
}
END_TEST

START_TEST(test_determinant_4x4) {
  test_start("test_determinant_4x4");
  matrix_t A;
  double result;
  s21_create_matrix(4, 4, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 0.0;
  A.matrix[0][2] = 0.0;
  A.matrix[0][3] = 0.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 2.0;
  A.matrix[1][2] = 0.0;
  A.matrix[1][3] = 0.0;
  A.matrix[2][0] = 0.0;
  A.matrix[2][1] = 0.0;
  A.matrix[2][2] = 3.0;
  A.matrix[2][3] = 0.0;
  A.matrix[3][0] = 0.0;
  A.matrix[3][1] = 0.0;
  A.matrix[3][2] = 0.0;
  A.matrix[3][3] = 4.0;

  int status = s21_determinant(&A, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result, 24.0, 1e-7);

  s21_remove_matrix(&A);
  test_end("test_determinant_4x4");
}
END_TEST

Suite* determinant_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("determinant");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_determinant_2x2);
  tcase_add_test(tc_core, test_determinant_3x3);
  tcase_add_test(tc_core, test_determinant_1x1);
  tcase_add_test(tc_core, test_determinant_non_square);
  tcase_add_test(tc_core, test_determinant_null_A);
  tcase_add_test(tc_core, test_determinant_null_result);
  tcase_add_test(tc_core, test_determinant_zero);
  tcase_add_test(tc_core, test_determinant_4x4);

  suite_add_tcase(s, tc_core);

  return s;
}