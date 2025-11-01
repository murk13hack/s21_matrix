# =============================================================================
# Project Structure
# =============================================================================
INCLUDE_DIR = include
SOURCE_DIR = src
TEST_DIR = tests
BUILD_DIR = build
TEST_OBJ_DIR = $(BUILD_DIR)/obj/${TEST_DIR}
TEST_BIN_DIR = $(BUILD_DIR)/bin/${TEST_DIR}
MAIN_OBJ_DIR = $(BUILD_DIR)/obj/main
MAIN_BIN_DIR = $(BUILD_DIR)/bin/main
GCOV_DIR = $(BUILD_DIR)/gcov_report
GCOV_DATA_DIR = $(GCOV_DIR)/data
LIB_DIR = $(BUILD_DIR)/lib

# =============================================================================
# Creating Directories
# =============================================================================
$(BUILD_DIR) $(TEST_OBJ_DIR) $(TEST_BIN_DIR) $(MAIN_OBJ_DIR) $(MAIN_BIN_DIR) $(GCOV_DIR) $(LIB_DIR) $(GCOV_DATA_DIR):
	mkdir -p $@

# =============================================================================
# Compiler Configuration
# =============================================================================
CC				::=		gcc
CFLAGS			::=		-Wall -Werror -Wextra -std=c11 -pedantic -I./include

TST_FLAG		::=		$(shell pkg-config --cflags --libs check)
TST_CFLAGS		::=		${CFLAGS} ${TST_FLAG}

GCOV_FLAGS		::=		-fprofile-arcs -ftest-coverage
LDFLAGS 		::=     -lgcov

DBG_FLAGS		::=		-g
REL_FLAG		::=		-DNDEBUG -O2

AR 				::= 	ar rcs
RANLIB 			::= 	ranlib

.PHONY: all test gcov_report view_report s21_matrix.a clean test_implemented test_function build_if_available format format-check

# =============================================================================
# Main Source, Objs list, BIN target, LIB
# =============================================================================
SRC = $(wildcard $(SOURCE_DIR)/*.c) $(wildcard $(SOURCE_DIR)/helpers/*.c)
HELPERS_SRC = $(wildcard $(SOURCE_DIR)/helpers/*.c)
MAIN_SRC = $(filter-out $(HELPERS_SRC), $(SRC))
MAIN_OBJS = $(patsubst $(SOURCE_DIR)/%.c, $(MAIN_OBJ_DIR)/%.o, $(MAIN_SRC)) \
            $(patsubst $(SOURCE_DIR)/helpers/%.c, $(MAIN_OBJ_DIR)/helpers_%.o, $(HELPERS_SRC))
MAIN_BIN = $(MAIN_BIN_DIR)/main
STATIC_LIB = $(LIB_DIR)/s21_matrix.a
# =============================================================================
# Test Source, Objs list, BIN target
# =============================================================================
TESTS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c, $(TEST_OBJ_DIR)/%.o, $(TESTS))
TEST_BIN = $(TEST_BIN_DIR)/test

# =============================================================================
# flags for special checking
# =============================================================================
CPPCHECK_FLAGS = --enable=all --inconclusive --std=c11 --language=c \
                 --force --check-config --suppress=missingIncludeSystem \
                 --error-exitcode=1 -I $(INCLUDE_DIR)
VALGRIND_FLAGS = --tool=memcheck --leak-check=full --show-leak-kinds=all \
                 --track-origins=yes --error-exitcode=1

# =============================================================================
# Compilation And Linking Of The Main Program
# =============================================================================
all: ${MAIN_BIN}
	@echo "Running static analysis (cppcheck)..."
	@cppcheck $(CPPCHECK_FLAGS) $(SOURCE_DIR) $(INCLUDE_DIR) $(TEST_DIR)
	@echo "Running valgrind memory check..."
	@valgrind $(VALGRIND_FLAGS) ./${MAIN_BIN}
	@echo "✅ All checks passed!"

${MAIN_BIN}: ${MAIN_OBJS} | ${MAIN_BIN_DIR}
	${CC} ${MAIN_OBJS} -o $@

${MAIN_OBJ_DIR}/%.o: ${SOURCE_DIR}/%.c | ${MAIN_OBJ_DIR}
	${CC} ${CFLAGS} -c $< -o $@

${MAIN_OBJ_DIR}/helpers_%.o: ${SOURCE_DIR}/helpers/%.c | ${MAIN_OBJ_DIR}
	${CC} ${CFLAGS} -c $< -o $@

# =============================================================================
# Create Static LIB
# =============================================================================
s21_matrix.a: $(STATIC_LIB)

$(STATIC_LIB): $(MAIN_OBJS) | $(LIB_DIR)
	$(AR) $@ $(MAIN_OBJS)
	$(RANLIB) $@

# =============================================================================
# Compilation And Linking Of The Tests Covering The Main Program
# =============================================================================
test: ${TEST_BIN}
	@echo "Running static analysis (cppcheck)..."
	@cppcheck $(CPPCHECK_FLAGS) $(SOURCE_DIR) $(INCLUDE_DIR) $(TEST_DIR)
	@echo "Running tests under Valgrind..."
	@valgrind $(VALGRIND_FLAGS) ./${TEST_BIN}
	@echo "✅ Tests and memory checks passed!"

${TEST_BIN}: ${TEST_OBJS} ${MAIN_OBJS} | ${TEST_BIN_DIR}
	${CC} ${TEST_OBJS} ${MAIN_OBJS} -o $@ ${TST_FLAG}

${TEST_OBJ_DIR}/%.o: ${TEST_DIR}/%.c | ${TEST_OBJ_DIR}
	${CC} ${TST_CFLAGS} -c $< -o $@

# =============================================================================
# Gcov Library Report
# =============================================================================
GCOV_ENV = GCOV_PREFIX=$(GCOV_DATA_DIR)
gcov_report: $(TEST_BIN) | $(GCOV_DIR)
	$(GCOV_ENV) ./$(TEST_BIN)
# Сбор данных coverage
	lcov -t "s21_matrix -> Tests Coverage" -o $(GCOV_DIR)/coverage.info -c -d $(GCOV_DATA_DIR) --rc lcov_branch_coverage=1
# Генерируем HTML отчет
	genhtml $(GCOV_DIR)/coverage.info -o $(GCOV_DIR)/html --rc genhtml_branch_coverage=1
# Показываем краткую статистику
	@echo "========================================="
	@echo "Coverage report generated in: $(GCOV_DIR)/html/"
	@echo "Open: $(GCOV_DIR)/html/index.html or call target {view_report}"
	@echo "========================================="

view_report: gcov_report
	xdg-open $(GCOV_DIR)/html/index.html 2>/dev/null || open $(GCOV_DIR)/html/index.html 2>/dev/null || echo "Open manually: $(GCOV_DIR)/html/index.html"

# =============================================================================
# Detailed information about the project
# =============================================================================
info:
	@echo "========================================="
	@echo "PROJECT STRUCTURE INFO"
	@echo "========================================="
	@echo "Build directory: $(BUILD_DIR)"
	@echo "Source directory: $(SOURCE_DIR)"
	@echo "Test directory: $(TEST_DIR)"
	@echo "Include directory: $(INCLUDE_DIR)"
	@echo ""
	@echo "COMPILER CONFIGURATION"
	@echo "========================================="
	@echo "Compiler: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "Test CFLAGS: $(TST_CFLAGS)"
	@echo "Test LIBS: $(TST_LIBS)"
	@echo ""
	@echo "FILES INFO"
	@echo "========================================="
	@echo "Sources (SRC):"
	@for file in $(SRC); do echo "$$file"; done
	@echo ""
	@echo "Objects (OBJS):"
	@for file in $(MAIN_OBJS); do echo "$$file"; done
	@echo ""
	@echo "Tests (TESTS):"
	@for file in $(TESTS); do echo "$$file"; done
	@echo ""
	@echo "TARGETS INFO"
	@echo "========================================="
	@echo "Main binary: $(MAIN_BIN)"
	@echo "Test runner: $(TEST_BIN)"
	@echo "Static library: $(STATIC_LIB)"
	@echo "Coverage dir: $(GCOV_DIR)"
	@echo "========================================="

# =============================================================================
# Brief information about the files
# =============================================================================
info_files:
	@echo "  Files summary:"
	@echo "  Sources: $(words $(SRC)) files"
	@echo "  Tests: $(words $(TESTS)) files" 
	@echo "  Objects: $(words $(MAIN_OBJS)) files"

# =============================================================================
# Information about the library
# =============================================================================
info_lib: $(STATIC_LIB)
	@echo "========================================="
	@echo "STATIC LIBRARY INFO: $(STATIC_LIB)"
	@echo "========================================="
	@echo "Size: $$(stat -f%z $(STATIC_LIB) 2>/dev/null || stat -c%s $(STATIC_LIB) 2>/dev/null || echo "unknown") bytes"
	@echo "Object files in library:"
	@ar -t $(STATIC_LIB) | while read file; do echo "  $$file"; done
	@echo ""
	@echo "Public symbols:"
	@nm -g $(STATIC_LIB) | grep -E "T|D" | head -10 | while read line; do echo "  $$line"; done
	@echo "========================================="

# =============================================================================
# Running tests by name (pattern matching)
# =============================================================================
test_%: $(TEST_BIN)
	@echo "Running cppcheck before test suite: $*"
	@cppcheck $(CPPCHECK_FLAGS) $(SOURCE_DIR) $(INCLUDE_DIR) $(TEST_DIR)
	@echo "Running test suite '$*' under Valgrind..."
	@valgrind $(VALGRIND_FLAGS) ./${TEST_BIN} -s $*
	@echo "✅ Tests and memory checks passed!"

# =============================================================================
# Selective testing based on available source files
# =============================================================================
# Check which source files exist and only test those
AVAILABLE_SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
AVAILABLE_TESTS = $(wildcard $(TEST_DIR)/*.c)

# Test only implemented functions
test_implemented: $(TEST_BIN)
	@echo "========================================="
	@echo "TESTING IMPLEMENTED FUNCTIONS ONLY"
	@echo "========================================="
	@echo "Available source files:"
	@for file in $(AVAILABLE_SOURCES); do echo "  $$file"; done
	@echo ""
	@echo "Available test files:"
	@for file in $(AVAILABLE_TESTS); do echo "  $$file"; done
	@echo ""
	@if [ -z "$(AVAILABLE_SOURCES)" ]; then \
		echo "⚠️  No source files found in $(SOURCE_DIR)/"; \
		echo "   Create source files first before running tests."; \
		exit 1; \
	fi
	@if [ -z "$(AVAILABLE_TESTS)" ]; then \
		echo "⚠️  No test files found in $(TEST_DIR)/"; \
		echo "   Create test files first before running tests."; \
		exit 1; \
	fi
	@echo "Running static analysis (cppcheck)..."
	@cppcheck $(CPPCHECK_FLAGS) $(SOURCE_DIR) $(INCLUDE_DIR) $(TEST_DIR)
	@echo "Running tests under Valgrind..."
	@valgrind $(VALGRIND_FLAGS) ./${TEST_BIN}
	@echo "✅ Tests and memory checks passed!"

# Test specific function (e.g., make test_function create_matrix)
test_function: $(TEST_BIN)
	@if [ -z "$(FUNC)" ]; then \
		echo "Usage: make test_function FUNC=function_name"; \
		echo "Available functions:"; \
		@for file in $(AVAILABLE_SOURCES); do \
			basename $$file .c | sed 's/s21_//' | sed 's/^/  /'; \
		done; \
		exit 1; \
	fi
	@echo "Running tests for function: $(FUNC)"
	@valgrind $(VALGRIND_FLAGS) ./${TEST_BIN} -s $(FUNC)

# Build and test only if source files exist
build_if_available: 
	@if [ -n "$(AVAILABLE_SOURCES)" ]; then \
		echo "Building with available sources..."; \
		make s21_matrix.a; \
	else \
		echo "⚠️  No source files found. Skipping build."; \
	fi

# =============================================================================
# List of all available tests
# =============================================================================
test_list: $(TEST_BIN)
	@echo "Available test suites:"
	@./$(TEST_BIN) --list | grep -E "^Suite:" | sed 's/Suite:/* /' || echo "  No test suites found"

# =============================================================================
# Code formatting targets
# =============================================================================
format:
	@echo "🔧 Formatting code with clang-format..."
	@if command -v clang-format >/dev/null 2>&1; then \
		find $(SOURCE_DIR) $(INCLUDE_DIR) $(TEST_DIR) -name '*.c' -o -name '*.h' 2>/dev/null | xargs clang-format -i; \
		echo "✅ Code formatted successfully"; \
	else \
		echo "❌ clang-format not found. Please install it:"; \
		echo "   Ubuntu/Debian: sudo apt-get install clang-format"; \
		echo "   macOS: brew install clang-format"; \
		exit 1; \
	fi

format-check:
	@echo "🔍 Checking code formatting..."
	@if command -v clang-format >/dev/null 2>&1; then \
		if find $(SOURCE_DIR) $(INCLUDE_DIR) $(TEST_DIR) -name '*.c' -o -name '*.h' 2>/dev/null | xargs clang-format --dry-run --Werror; then \
			echo "✅ Code formatting is correct"; \
		else \
			echo "❌ Code formatting issues found. Run 'make format' to fix."; \
			exit 1; \
		fi; \
	else \
		echo "❌ clang-format not found. Please install it."; \
		exit 1; \
	fi

# =============================================================================
# Cleaning build dir
# =============================================================================
clean: 
	rm -rf ${BUILD_DIR}
	find . -name "*.gcno" -delete
	find . -name "*.gcda" -delete
	find . -name "*.gcov" -delete
