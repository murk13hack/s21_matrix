#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/s21_matrix.h"

// Test suite for s21_create_matrix function
START_TEST(test_create_matrix_valid) {
    matrix_t result;
    int status = s21_create_matrix(3, 4, &result);
    
    ck_assert_int_eq(status, S21_OK);
    ck_assert_int_eq(result.rows, 3);
    ck_assert_int_eq(result.columns, 4);
    ck_assert_ptr_nonnull(result.matrix);
    
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_create_matrix_zero_rows) {
    matrix_t result;
    int status = s21_create_matrix(0, 4, &result);
    
    ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_create_matrix_zero_columns) {
    matrix_t result;
    int status = s21_create_matrix(3, 0, &result);
    
    ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_create_matrix_null_result) {
    int status = s21_create_matrix(3, 4, NULL);
    
    ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_create_matrix_negative_rows) {
    matrix_t result;
    int status = s21_create_matrix(-1, 4, &result);
    
    ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_create_matrix_negative_columns) {
    matrix_t result;
    int status = s21_create_matrix(3, -1, &result);
    
    ck_assert_int_eq(status, S21_ERROR_INCORRECT_MATRIX);
}
END_TEST

Suite *create_matrix_suite(void) {
    Suite *s;
    TCase *tc_core;

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
