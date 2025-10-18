# CI/CD YAML File Explanation

This document provides a line-by-line explanation of our `ci.yml` file, explaining what each part does and why it's needed.

## Complete YAML File

```yaml
name: C Project CI

on:
  push:
    branches: [ main, master, dev, develop ]
  pull_request:
    branches: [ main, master, dev, develop ]

jobs:
  build-and-test:
    runs-on: ubuntu-latest

    steps:
      # 1. Клонирование репозитория
      - name: Checkout repository
        uses: actions/checkout@v4

      # 2. Кэширование пакетов apt (с правильными правами)
      - name: Cache apt packages
        uses: actions/cache@v3
        with:
          path: |
            /var/lib/apt/lists
            /var/cache/apt/pkgcache.bin
            /var/cache/apt/srcpkgcache.bin
          key: ${{ runner.os }}-apt-${{ hashFiles('**/apt-packages.txt') }}
          restore-keys: |
            ${{ runner.os }}-apt-

      # 3. Установка зависимостей (с кэшированием)
      - name: Install dependencies
        run: |
          # Create package list for caching
          echo "make gcc valgrind cppcheck clang-format check" > apt-packages.txt
          
          # Make apt directories readable for caching
          sudo chmod -R a+r /var/lib/apt/lists/ 2>/dev/null || true
          sudo chmod -R a+r /var/cache/apt/ 2>/dev/null || true
          
          # Update package list only if cache miss
          if [ ! -f /var/cache/apt/pkgcache.bin ]; then
            echo "📦 Cache miss - updating package lists..."
            sudo apt-get update -qq
          else
            echo "✅ Using cached package lists..."
          fi
          
          # Install packages
          sudo apt-get install -y -qq --no-install-recommends \
            make gcc valgrind cppcheck clang-format check

      # 4. Проверка наличия исходных файлов
      - name: Check for source files
        run: |
          echo "🔍 Checking for source files..."
          if [ ! -d "src" ] || [ -z "$(find src -name '*.c' 2>/dev/null)" ]; then
            echo "❌ No source files found in src/ directory"
            echo "   Source files are required for this project."
            echo "   Please add source files to enable testing."
            exit 1
          fi
          echo "✅ Source files found:"
          find src -name '*.c' | sed 's/^/  /'
        shell: bash

      # 5. Форматирование и проверка стиля кода (clang-format)
      - name: Format and check code style (clang-format)
        run: |
          echo "🔍 Formatting and checking code style..."
          clang-format --version
          # Format all C and H files
          find src include tests -name '*.c' -o -name '*.h' 2>/dev/null | xargs clang-format -i
          echo "✅ Code formatted successfully"
          # Check if formatting changed anything (should be clean after formatting)
          if ! git diff --quiet; then
            echo "❌ Code formatting issues found. Please run 'clang-format -i' on your files."
            git diff
            exit 1
          fi
          echo "✅ Code style check passed"
        shell: bash

      # 6. Статический анализ (cppcheck)
      - name: Run static analysis (cppcheck)
        run: |
          echo "🧠 Running cppcheck..."
          cppcheck --enable=all --inconclusive --std=c11 --language=c \
            --force --check-config --suppress=missingIncludeSystem \
            --error-exitcode=1 -I include src tests
        shell: bash

      # 7. Сборка проекта
      - name: Build project
        run: |
          echo "🏗️ Building project..."
          make clean
          make build_if_available
        shell: bash

      # 8. Запуск тестов
      - name: Run tests
        run: |
          echo "🧪 Running tests..."
          if [ ! -d "tests" ] || [ -z "$(find tests -name '*.c' 2>/dev/null)" ]; then
            echo "❌ No test files found in tests/ directory"
            echo "   Test files are required for this project."
            echo "   Please add test files to enable testing."
            exit 1
          fi
          make test_implemented
        shell: bash

      # 9. Проверка утечек памяти
      - name: Memory check (valgrind)
        run: |
          echo "🔬 Checking for memory leaks..."
          if [ ! -f "build/bin/tests/test" ]; then
            echo "❌ Test binary not found - build or tests failed"
            exit 1
          fi
          valgrind --leak-check=full --error-exitcode=1 ./build/bin/tests/test
        shell: bash
```

## Line-by-Line Explanation

### Header Section

```yaml
name: C Project CI
```
**What**: Names the workflow
**Why**: Shows up in GitHub Actions UI

```yaml
on:
  push:
    branches: [ main, master, dev, develop ]
  pull_request:
    branches: [ main, master, dev, develop ]
```
**What**: Defines when the workflow runs
**Why**: 
- `push`: Runs when code is pushed to main branches
- `pull_request`: Runs when PRs are created/updated
- `branches`: Specifies which branches trigger the workflow

### Job Definition

```yaml
jobs:
  build-and-test:
    runs-on: ubuntu-latest
```
**What**: Defines a job named "build-and-test"
**Why**: 
- `jobs`: Top-level container for all jobs
- `build-and-test`: Job name (shows in UI)
- `runs-on`: Specifies the runner environment (Ubuntu latest)

### Steps Section

```yaml
steps:
```
**What**: Container for all steps in the job
**Why**: Steps run sequentially in order

### Step 1: Checkout

```yaml
- name: Checkout repository
  uses: actions/checkout@v4
```
**What**: Downloads repository code
**Why**: Runner needs access to source code
**Details**:
- `-`: YAML list item
- `name`: Human-readable step name
- `uses`: Uses a pre-built action
- `actions/checkout@v4`: Official GitHub checkout action

### Step 2: Cache Setup

```yaml
- name: Cache apt packages
  uses: actions/cache@v3
  with:
    path: |
      /var/lib/apt/lists
      /var/cache/apt/pkgcache.bin
      /var/cache/apt/srcpkgcache.bin
    key: ${{ runner.os }}-apt-${{ hashFiles('**/apt-packages.txt') }}
    restore-keys: |
      ${{ runner.os }}-apt-
```
**What**: Sets up caching for apt packages
**Why**: Speeds up subsequent runs
**Details**:
- `with`: Parameters for the action
- `path`: Files/directories to cache
- `key`: Unique identifier for this cache
- `restore-keys`: Fallback keys if exact match not found
- `${{ }}`: GitHub Actions expression syntax
- `runner.os`: Operating system (ubuntu-latest)
- `hashFiles()`: Creates hash of file contents

### Step 3: Install Dependencies

```yaml
- name: Install dependencies
  run: |
    # Create package list for caching
    echo "make gcc valgrind cppcheck clang-format check" > apt-packages.txt
    
    # Make apt directories readable for caching
    sudo chmod -R a+r /var/lib/apt/lists/ 2>/dev/null || true
    sudo chmod -R a+r /var/cache/apt/ 2>/dev/null || true
    
    # Update package list only if cache miss
    if [ ! -f /var/cache/apt/pkgcache.bin ]; then
      echo "📦 Cache miss - updating package lists..."
      sudo apt-get update -qq
    else
      echo "✅ Using cached package lists..."
    fi
    
    # Install packages
    sudo apt-get install -y -qq --no-install-recommends \
      make gcc valgrind cppcheck clang-format check
```
**What**: Installs required tools
**Why**: Need tools to build and test C code
**Details**:
- `run: |`: Multi-line shell script
- `echo "..." > file`: Creates file with content
- `sudo chmod -R a+r`: Makes directories readable
- `2>/dev/null || true`: Suppresses errors safely
- `if [ ! -f file ]`: Checks if file doesn't exist
- `sudo apt-get update -qq`: Updates package lists quietly
- `sudo apt-get install -y -qq --no-install-recommends`: Installs packages quietly without recommendations

### Step 4: Check Source Files

```yaml
- name: Check for source files
  run: |
    echo "🔍 Checking for source files..."
    if [ ! -d "src" ] || [ -z "$(find src -name '*.c' 2>/dev/null)" ]; then
      echo "❌ No source files found in src/ directory"
      echo "   Source files are required for this project."
      echo "   Please add source files to enable testing."
      exit 1
    fi
    echo "✅ Source files found:"
    find src -name '*.c' | sed 's/^/  /'
  shell: bash
```
**What**: Validates source files exist
**Why**: Prevents CI from running on empty repositories
**Details**:
- `[ ! -d "src" ]`: Checks if src directory doesn't exist
- `[ -z "$(command)" ]`: Checks if command output is empty
- `find src -name '*.c'`: Finds all .c files in src directory
- `2>/dev/null`: Suppresses error output
- `exit 1`: Exits with error code
- `sed 's/^/  /'`: Adds two spaces to beginning of each line

### Step 5: Code Formatting

```yaml
- name: Format and check code style (clang-format)
  run: |
    echo "🔍 Formatting and checking code style..."
    clang-format --version
    # Format all C and H files
    find src include tests -name '*.c' -o -name '*.h' 2>/dev/null | xargs clang-format -i
    echo "✅ Code formatted successfully"
    # Check if formatting changed anything (should be clean after formatting)
    if ! git diff --quiet; then
      echo "❌ Code formatting issues found. Please run 'clang-format -i' on your files."
      git diff
      exit 1
    fi
    echo "✅ Code style check passed"
  shell: bash
```
**What**: Formats code and checks style
**Why**: Ensures consistent code formatting
**Details**:
- `clang-format --version`: Shows version info
- `find ... -name '*.c' -o -name '*.h'`: Finds C and H files
- `xargs clang-format -i`: Runs clang-format on each file
- `git diff --quiet`: Checks if there are uncommitted changes
- `git diff`: Shows the differences

### Step 6: Static Analysis

```yaml
- name: Run static analysis (cppcheck)
  run: |
    echo "🧠 Running cppcheck..."
    cppcheck --enable=all --inconclusive --std=c11 --language=c \
      --force --check-config --suppress=missingIncludeSystem \
      --error-exitcode=1 -I include src tests
  shell: bash
```
**What**: Analyzes code for bugs
**Why**: Catches potential issues before runtime
**Details**:
- `--enable=all`: Enable all checks
- `--inconclusive`: Show inconclusive results
- `--std=c11`: Use C11 standard
- `--language=c`: Specify C language
- `--force`: Force checking even with errors
- `--check-config`: Check configuration
- `--suppress=missingIncludeSystem`: Ignore missing system headers
- `--error-exitcode=1`: Exit with error if issues found
- `-I include`: Add include directory to search path

### Step 7: Build Project

```yaml
- name: Build project
  run: |
    echo "🏗️ Building project..."
    make clean
    make build_if_available
  shell: bash
```
**What**: Builds the project
**Why**: Ensures code compiles correctly
**Details**:
- `make clean`: Removes old build artifacts
- `make build_if_available`: Builds only if source files exist

### Step 8: Run Tests

```yaml
- name: Run tests
  run: |
    echo "🧪 Running tests..."
    if [ ! -d "tests" ] || [ -z "$(find tests -name '*.c' 2>/dev/null)" ]; then
      echo "❌ No test files found in tests/ directory"
      echo "   Test files are required for this project."
      echo "   Please add test files to enable testing."
      exit 1
    fi
    make test_implemented
  shell: bash
```
**What**: Runs unit tests
**Why**: Ensures code works correctly
**Details**:
- Similar validation to source files check
- `make test_implemented`: Runs tests using Makefile

### Step 9: Memory Leak Detection

```yaml
- name: Memory check (valgrind)
  run: |
    echo "🔬 Checking for memory leaks..."
    if [ ! -f "build/bin/tests/test" ]; then
      echo "❌ Test binary not found - build or tests failed"
      exit 1
    fi
    valgrind --leak-check=full --error-exitcode=1 ./build/bin/tests/test
  shell: bash
```
**What**: Checks for memory leaks
**Why**: Ensures no memory management issues
**Details**:
- `[ ! -f "file" ]`: Checks if file doesn't exist
- `valgrind --leak-check=full`: Full memory leak detection
- `--error-exitcode=1`: Exit with error if leaks found
- `./build/bin/tests/test`: Path to test executable

## Key Concepts Explained

### GitHub Actions Expressions

- `${{ }}`: Evaluates expressions
- `${{ runner.os }}`: Operating system
- `${{ github.workspace }}`: Workspace directory
- `${{ hashFiles('pattern') }}`: Hash of matching files

### Shell Scripting

- `||`: OR operator (run second command if first fails)
- `&&`: AND operator (run second command if first succeeds)
- `2>/dev/null`: Redirect stderr to /dev/null
- `|`: Pipe (send output to next command)
- `>`: Redirect output to file
- `>>`: Append output to file

### File Operations

- `[ -d dir ]`: Check if directory exists
- `[ -f file ]`: Check if file exists
- `[ -z string ]`: Check if string is empty
- `find`: Search for files
- `xargs`: Execute command for each input

### Error Handling

- `exit 1`: Exit with error code
- `|| true`: Always succeed (suppress errors)
- `2>/dev/null`: Hide error messages

## Common Patterns

### Conditional Execution

```bash
if [ condition ]; then
  echo "True"
else
  echo "False"
fi
```

### File Existence Check

```bash
if [ ! -f "file" ]; then
  echo "File doesn't exist"
  exit 1
fi
```

### Command Chaining

```bash
command1 && command2  # Run command2 only if command1 succeeds
command1 || command2  # Run command2 only if command1 fails
```

### Output Suppression

```bash
command 2>/dev/null    # Hide errors
command >/dev/null      # Hide output
command >/dev/null 2>&1 # Hide everything
```

This YAML file creates a comprehensive CI pipeline that ensures code quality, builds successfully, runs tests, and checks for memory leaks - all automatically triggered by code changes.
