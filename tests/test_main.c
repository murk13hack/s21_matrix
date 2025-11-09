#include <check.h>
#include <stdio.h>
#include <stdlib.h>

Suite* create_matrix_suite(void);
Suite* remove_matrix_suite(void);
Suite* eq_matrix_suite(void);
Suite* sum_matrix_suite(void);
Suite* sub_matrix_suite(void);
Suite* mult_number_suite(void);
Suite* mult_matrix_suite(void);
Suite* transpose_suite(void);
Suite* calc_complements_suite(void);
Suite* determinant_suite(void);
Suite* inverse_matrix_suite(void);

void test_start(const char* test_name) {
    printf("▶ START: %s\n", test_name);
    fflush(stdout);
}

void test_end(const char* test_name) {
    printf("✓ END: %s\n\n", test_name);
    fflush(stdout);
}

int main(void) {
  int number_failed;
  SRunner* sr;

  printf("🚀 Starting test execution...\n");
  printf("==============================\n\n");

  sr = srunner_create(NULL);

  srunner_add_suite(sr, create_matrix_suite());
  srunner_add_suite(sr, remove_matrix_suite());
  srunner_add_suite(sr, eq_matrix_suite());
  srunner_add_suite(sr, sum_matrix_suite());
  srunner_add_suite(sr, sub_matrix_suite());
  srunner_add_suite(sr, mult_number_suite());
  srunner_add_suite(sr, mult_matrix_suite());
  srunner_add_suite(sr, transpose_suite());
  srunner_add_suite(sr, calc_complements_suite());
  srunner_add_suite(sr, determinant_suite());
  srunner_add_suite(sr, inverse_matrix_suite());

  srunner_set_log(sr, "test_log.txt");

  printf("📊 Running tests...\n");
  printf("==============================\n\n");

  srunner_run_all(sr, CK_NORMAL);
  
  printf("==============================\n");
  printf("📋 Test execution completed\n");

  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  if (number_failed == 0) {
    printf("✅ ALL TESTS PASSED\n");
  } else {
    printf("❌ %d TEST(S) FAILED\n", number_failed);
  }

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}