#include <check.h>
#include <stdlib.h>
#include <stdio.h>

// Include test suites
Suite *create_matrix_suite(void);
Suite *remove_matrix_suite(void);
Suite *eq_matrix_suite(void);

int main(void) {
    int number_failed;
    SRunner *sr;

    sr = srunner_create(NULL);

    // Add test suites here as you implement functions
    srunner_add_suite(sr, create_matrix_suite());
    srunner_add_suite(sr, remove_matrix_suite());
    srunner_add_suite(sr, eq_matrix_suite());

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
