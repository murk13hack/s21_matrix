#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/s21_matrix.h"

// Объявление функций логирования
void test_start(const char* test_name);
void test_end(const char* test_name);

// Test suite for s21_create_matrix function
START_TEST(test_create_matrix_valid) {
  test_start("test_create_matrix_valid");
  matrix_t result;
  int status = s21_create_matrix(3, 4, &result);

  ck_assert_int_eq(status, S21_OK);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 4);
  ck_assert_ptr_nonnull(result.matrix);

  s21_remove_matrix(&result);
  test_end("test_create_matrix_valid");
}
END_TEST

START_TEST(test_create_matrix_zero_rows) {
  test_start("test_create_matrix_zero_rows");
  matrix_t result;
  int status = s21_create_matrix(0, 4, &result);

  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
  test_end("test_create_matrix_zero_rows");
}
END_TEST

START_TEST(test_create_matrix_zero_columns) {
  test_start("test_create_matrix_zero_columns");
  matrix_t result;
  int status = s21_create_matrix(3, 0, &result);

  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
  test_end("test_create_matrix_zero_columns");
}
END_TEST

START_TEST(test_create_matrix_null_result) {
  test_start("test_create_matrix_null_result");
  int status = s21_create_matrix(3, 4, NULL);

  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
  test_end("test_create_matrix_null_result");
}
END_TEST

START_TEST(test_create_matrix_negative_rows) {
  test_start("test_create_matrix_negative_rows");
  matrix_t result;
  int status = s21_create_matrix(-1, 4, &result);

  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
  test_end("test_create_matrix_negative_rows");
}
END_TEST

START_TEST(test_create_matrix_negative_columns) {
  test_start("test_create_matrix_negative_columns");
  matrix_t result;
  int status = s21_create_matrix(3, -1, &result);

  ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
  test_end("test_create_matrix_negative_columns");
}
END_TEST

Suite* create_matrix_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("create_matrix");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_create_matrix_valid);
  tcase_add_test(tc_core, test_create_matrix_zero_rows);
  tcase_add_test(tc_core, test_create_matrix_zero_columns);
  tcase_add_test(tc_core, test_create_matrix_null_result);
  tcase_add_test(tc_core, test_create_matrix_negative_rows);
  tcase_add_test(tc_core, test_create_matrix_negative_columns);

  suite_add_tcase(s, tc_core);

  return s;
}