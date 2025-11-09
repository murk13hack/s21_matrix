#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/s21_matrix.h"

// Объявление функций логирования
void test_start(const char* test_name);
void test_end(const char* test_name);

// Test suite for s21_eq_matrix function
START_TEST(test_eq_matrix_identical) {
  test_start("test_eq_matrix_identical");
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  // Fill matrices with identical values
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;

  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  test_end("test_eq_matrix_identical");
}
END_TEST

START_TEST(test_eq_matrix_different_values) {
  test_start("test_eq_matrix_different_values");
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  // Fill matrices with different values
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 5.0;  // Different value

  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  test_end("test_eq_matrix_different_values");
}
END_TEST

START_TEST(test_eq_matrix_different_sizes) {
  test_start("test_eq_matrix_different_sizes");
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 3, &B);

  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  test_end("test_eq_matrix_different_sizes");
}
END_TEST

START_TEST(test_eq_matrix_null_pointers) {
  test_start("test_eq_matrix_null_pointers");
  matrix_t A;
  s21_create_matrix(2, 2, &A);

  int result = s21_eq_matrix(NULL, &A);
  ck_assert_int_eq(result, FAILURE);

  result = s21_eq_matrix(&A, NULL);
  ck_assert_int_eq(result, FAILURE);

  result = s21_eq_matrix(NULL, NULL);
  ck_assert_int_eq(result, FAILURE);

  s21_remove_matrix(&A);
  test_end("test_eq_matrix_null_pointers");
}
END_TEST

START_TEST(test_eq_matrix_precision) {
  test_start("test_eq_matrix_precision");
  matrix_t A, B;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);

  // Test precision up to 6 decimal places
  A.matrix[0][0] = 1.000001;
  B.matrix[0][0] = 1.000002;  // Difference in 6th decimal place

  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, FAILURE);

  // Test precision within tolerance
  A.matrix[0][0] = 1.0000001;
  B.matrix[0][0] = 1.0000002;  // Difference in 7th decimal place

  result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  test_end("test_eq_matrix_precision");
}
END_TEST

Suite* eq_matrix_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("eq_matrix");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_eq_matrix_identical);
  tcase_add_test(tc_core, test_eq_matrix_different_values);
  tcase_add_test(tc_core, test_eq_matrix_different_sizes);
  tcase_add_test(tc_core, test_eq_matrix_null_pointers);
  tcase_add_test(tc_core, test_eq_matrix_precision);

  suite_add_tcase(s, tc_core);

  return s;
}