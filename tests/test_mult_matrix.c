#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/s21_matrix.h"

// Test suite for s21_mult_matrix function
START_TEST(test_mult_matrix_valid) {
  matrix_t A, B, result;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  
  A.matrix[0][0] = 1.0; A.matrix[0][1] = 2.0; A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0; A.matrix[1][1] = 5.0; A.matrix[1][2] = 6.0;
  
  B.matrix[0][0] = 1.0; B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0; B.matrix[1][1] = 4.0;
  B.matrix[2][0] = 5.0; B.matrix[2][1] = 6.0;
  
  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 2);
  ck_assert_double_eq_tol(result.matrix[0][0], 22.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 28.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 49.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 64.0, 1e-7);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_incompatible_sizes) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(3, 2, &B);
  
  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_ERROR_CALCULATION);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_matrix_null_A) {
  matrix_t B, result;
  s21_create_matrix(2, 2, &B);
  
  int status = s21_mult_matrix(NULL, &B, &result);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
  
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_matrix_null_B) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);
  
  int status = s21_mult_matrix(&A, NULL, &result);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
  
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_mult_matrix_null_result) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  
  int status = s21_mult_matrix(&A, &B, NULL);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_matrix_square) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  
  A.matrix[0][0] = 1.0; A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0; A.matrix[1][1] = 4.0;
  
  B.matrix[0][0] = 5.0; B.matrix[0][1] = 6.0;
  B.matrix[1][0] = 7.0; B.matrix[1][1] = 8.0;
  
  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 19.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 22.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 43.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 50.0, 1e-7);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_identity) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  
  A.matrix[0][0] = 1.0; A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0; A.matrix[1][1] = 4.0;
  
  // Identity matrix
  B.matrix[0][0] = 1.0; B.matrix[0][1] = 0.0;
  B.matrix[1][0] = 0.0; B.matrix[1][1] = 1.0;
  
  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], A.matrix[0][0], 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], A.matrix[0][1], 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], A.matrix[1][0], 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], A.matrix[1][1], 1e-7);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

Suite* mult_matrix_suite(void) {
  Suite* s;
  TCase* tc_core;
  
  s = suite_create("mult_matrix");
  tc_core = tcase_create("Core");
  
  tcase_add_test(tc_core, test_mult_matrix_valid);
  tcase_add_test(tc_core, test_mult_matrix_incompatible_sizes);
  tcase_add_test(tc_core, test_mult_matrix_null_A);
  tcase_add_test(tc_core, test_mult_matrix_null_B);
  tcase_add_test(tc_core, test_mult_matrix_null_result);
  tcase_add_test(tc_core, test_mult_matrix_square);
  tcase_add_test(tc_core, test_mult_matrix_identity);
  
  suite_add_tcase(s, tc_core);
  
  return s;
}
