#include <check.h>
#include <stdio.h>
#include <stdlib.h>

// Include test suites
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

int main(void) {
  int number_failed;
  SRunner* sr;

  sr = srunner_create(NULL);

  // Add test suites for all matrix functions
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

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
