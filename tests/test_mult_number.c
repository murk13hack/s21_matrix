#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/s21_matrix.h"

// Test suite for s21_mult_number function
START_TEST(test_mult_number_valid) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);
  
  A.matrix[0][0] = 1.0; A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0; A.matrix[1][1] = 4.0;
  
  int status = s21_mult_number(&A, 2.5, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 2.5, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 5.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 7.5, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 10.0, 1e-7);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_number_by_zero) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);
  
  A.matrix[0][0] = 1.0; A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0; A.matrix[1][1] = 4.0;
  
  int status = s21_mult_number(&A, 0.0, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 0.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 0.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 0.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 0.0, 1e-7);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_number_by_negative) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);
  
  A.matrix[0][0] = 1.0; A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0; A.matrix[1][1] = 4.0;
  
  int status = s21_mult_number(&A, -2.0, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], -2.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], -4.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], -6.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], -8.0, 1e-7);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_number_null_A) {
  matrix_t result;
  
  int status = s21_mult_number(NULL, 2.0, &result);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_mult_number_null_result) {
  matrix_t A;
  s21_create_matrix(2, 2, &A);
  
  int status = s21_mult_number(&A, 2.0, NULL);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
  
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_mult_number_by_one) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);
  
  A.matrix[0][0] = 1.0; A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0; A.matrix[1][1] = 4.0;
  
  int status = s21_mult_number(&A, 1.0, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 2.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 3.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 4.0, 1e-7);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_number_decimal) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);
  
  A.matrix[0][0] = 10.0; A.matrix[0][1] = 20.0;
  A.matrix[1][0] = 30.0; A.matrix[1][1] = 40.0;
  
  int status = s21_mult_number(&A, 0.1, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], 2.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], 3.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 4.0, 1e-7);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

Suite* mult_number_suite(void) {
  Suite* s;
  TCase* tc_core;
  
  s = suite_create("mult_number");
  tc_core = tcase_create("Core");
  
  tcase_add_test(tc_core, test_mult_number_valid);
  tcase_add_test(tc_core, test_mult_number_by_zero);
  tcase_add_test(tc_core, test_mult_number_by_negative);
  tcase_add_test(tc_core, test_mult_number_null_A);
  tcase_add_test(tc_core, test_mult_number_null_result);
  tcase_add_test(tc_core, test_mult_number_by_one);
  tcase_add_test(tc_core, test_mult_number_decimal);
  
  suite_add_tcase(s, tc_core);
  
  return s;
}
