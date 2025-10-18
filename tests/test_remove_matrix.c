#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/s21_matrix.h"

// Test suite for s21_remove_matrix function
START_TEST(test_remove_matrix_valid) {
  matrix_t matrix;
  int status = s21_create_matrix(3, 4, &matrix);
  ck_assert_int_eq(status, S21_OK);

  s21_remove_matrix(&matrix);

  // After removal, matrix should be in a clean state
  ck_assert_ptr_null(matrix.matrix);
  ck_assert_int_eq(matrix.rows, 0);
  ck_assert_int_eq(matrix.columns, 0);
}
END_TEST

START_TEST(test_remove_matrix_null_matrix) {
  // This should not crash
  s21_remove_matrix(NULL);
}
END_TEST

START_TEST(test_remove_matrix_already_null) {
  matrix_t matrix = {0};
  matrix.matrix = NULL;
  matrix.rows = 0;
  matrix.columns = 0;

  // This should not crash
  s21_remove_matrix(&matrix);
}
END_TEST

Suite* remove_matrix_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("remove_matrix");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_remove_matrix_valid);
  tcase_add_test(tc_core, test_remove_matrix_null_matrix);
  tcase_add_test(tc_core, test_remove_matrix_already_null);

  suite_add_tcase(s, tc_core);

  return s;
}
