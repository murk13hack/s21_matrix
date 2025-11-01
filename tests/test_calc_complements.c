#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/s21_matrix.h"

// Test suite for s21_calc_complements function
START_TEST(test_calc_complements_valid) {
  matrix_t A, result;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 2.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = 2.0;
  A.matrix[2][2] = 1.0;

  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_2x2) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 2);
  ck_assert_double_eq_tol(result.matrix[0][0], 4.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[0][1], -3.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][0], -2.0, 1e-7);
  ck_assert_double_eq_tol(result.matrix[1][1], 1.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_non_square) {
  matrix_t A, result;
  s21_create_matrix(2, 3, &A);

  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, S21_ERROR_CALCULATION);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_null_A) {
  matrix_t result;

  int status = s21_calc_complements(NULL, &result);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_calc_complements_null_result) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);

  int status = s21_calc_complements(&A, NULL);
  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_1x1) {
  matrix_t A, result;
  s21_create_matrix(1, 1, &A);

  A.matrix[0][0] = 5.0;

  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, S21_ERROR_CALCULATION);

  s21_remove_matrix(&A);
}
END_TEST

Suite* calc_complements_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("calc_complements");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_calc_complements_valid);
  tcase_add_test(tc_core, test_calc_complements_2x2);
  tcase_add_test(tc_core, test_calc_complements_non_square);
  tcase_add_test(tc_core, test_calc_complements_null_A);
  tcase_add_test(tc_core, test_calc_complements_null_result);
  tcase_add_test(tc_core, test_calc_complements_1x1);

  suite_add_tcase(s, tc_core);

  return s;
}
