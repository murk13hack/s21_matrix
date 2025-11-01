#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/s21_matrix.h"

// Test suite for s21_sub_matrix function
START_TEST(test_sub_matrix_valid) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 5.0;
  A.matrix[0][1] = 6.0;
  A.matrix[1][0] = 7.0;
  A.matrix[1][1] = 8.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;

  int status = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 4.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 4.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 4.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 4.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix_different_sizes) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 3, &B);

  int status = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_ERROR_CALCULATION);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sub_matrix_null_A) {
  matrix_t B, result;
  s21_create_matrix(2, 2, &B);

  int status = s21_sub_matrix(NULL, &B, &result);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);

  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sub_matrix_null_B) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);

  int status = s21_sub_matrix(&A, NULL, &result);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_sub_matrix_null_result) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  int status = s21_sub_matrix(&A, &B, NULL);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sub_matrix_negative_result) {
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

  int status = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], -4.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], -4.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], -4.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], -4.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix_zero_result) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 5.0;
  A.matrix[0][1] = 5.0;
  A.matrix[1][0] = 5.0;
  A.matrix[1][1] = 5.0;

  B.matrix[0][0] = 5.0;
  B.matrix[0][1] = 5.0;
  B.matrix[1][0] = 5.0;
  B.matrix[1][1] = 5.0;

  int status = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 0.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 0.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 0.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 0.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

Suite* sub_matrix_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("sub_matrix");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_sub_matrix_valid);
  tcase_add_test(tc_core, test_sub_matrix_different_sizes);
  tcase_add_test(tc_core, test_sub_matrix_null_A);
  tcase_add_test(tc_core, test_sub_matrix_null_B);
  tcase_add_test(tc_core, test_sub_matrix_null_result);
  tcase_add_test(tc_core, test_sub_matrix_negative_result);
  tcase_add_test(tc_core, test_sub_matrix_zero_result);

  suite_add_tcase(s, tc_core);

  return s;
}
