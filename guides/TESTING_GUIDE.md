# Selective Testing Guide

This project supports selective testing, allowing you to test only the functions you've implemented without requiring all functions to be complete.

## How It Works

The Makefile and CI/CD pipeline automatically detect which source files exist in the `src/` directory and only run tests for those functions.

## Available Make Targets

### `make test_implemented`
Runs tests only for functions that have both source files and test files available.

### `make test_function FUNC=function_name`
Runs tests for a specific function. Example:
```bash
make test_function FUNC=create_matrix
```

### `make build_if_available`
Builds the library only if source files exist.

### `make test_list`
Lists all available test suites.

## Directory Structure

```
src/           # Source files (s21_matrix.c, etc.)
tests/         # Test files (test_create_matrix.c, etc.)
include/       # Header files (s21_matrix.h)
```

## Workflow

1. **Start with basic functions**: Implement `s21_create_matrix`, `s21_remove_matrix`, and `s21_eq_matrix` first
2. **Create corresponding tests**: Add test files in `tests/` directory
3. **Test incrementally**: Use `make test_implemented` to test only what you've completed
4. **Push to CI**: The CI pipeline will automatically test only your implemented functions

## Example Implementation Order

1. `s21_create_matrix` - Matrix creation
2. `s21_remove_matrix` - Matrix cleanup
3. `s21_eq_matrix` - Matrix comparison
4. `s21_sum_matrix` - Matrix addition
5. `s21_sub_matrix` - Matrix subtraction
6. `s21_mult_number` - Scalar multiplication
7. `s21_mult_matrix` - Matrix multiplication
8. `s21_transpose` - Matrix transposition
9. `s21_calc_complements` - Algebraic complements
10. `s21_determinant` - Determinant calculation
11. `s21_inverse_matrix` - Inverse matrix

## CI/CD Behavior

- **No source files**: CI passes with warning message
- **Partial implementation**: CI tests only implemented functions
- **Full implementation**: CI runs complete test suite

This allows you to push incremental progress without breaking the build.
