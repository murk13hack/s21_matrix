# CI/CD Guide for C Projects

This guide explains how to set up a complete CI/CD pipeline for C projects using GitHub Actions, including code formatting, static analysis, building, testing, and memory leak detection.

## Table of Contents

1. [What is CI/CD?](#what-is-cicd)
2. [GitHub Actions Overview](#github-actions-overview)
3. [Our CI Pipeline Breakdown](#our-ci-pipeline-breakdown)
4. [Step-by-Step Explanation](#step-by-step-explanation)
5. [Performance Optimizations](#performance-optimizations)
6. [Troubleshooting](#troubleshooting)
7. [Best Practices](#best-practices)

## What is CI/CD?

**CI/CD** stands for **Continuous Integration** and **Continuous Deployment**:

- **Continuous Integration (CI)**: Automatically build and test code when changes are pushed
- **Continuous Deployment (CD)**: Automatically deploy code to production (not covered in this guide)

### Benefits of CI/CD

- ✅ **Catch bugs early** before they reach production
- ✅ **Ensure code quality** with automated checks
- ✅ **Prevent broken code** from being merged
- ✅ **Save time** with automated testing
- ✅ **Consistent builds** across different environments

## GitHub Actions Overview

GitHub Actions is a platform that allows you to automate workflows directly in your GitHub repository.

### Key Concepts

- **Workflow**: A configurable automated process
- **Job**: A set of steps that execute on the same runner
- **Step**: An individual task that can run commands or actions
- **Runner**: A server that runs your workflows
- **Action**: A reusable unit of code

### Workflow File Location

Workflows are defined in `.github/workflows/` directory as YAML files.

## Our CI Pipeline Breakdown

Our CI pipeline (`ci.yml`) performs the following checks on every push and pull request:

```yaml
name: C Project CI

on:
  push:
    branches: [ main, master, dev, develop ]
  pull_request:
    branches: [ main, master, dev, develop ]
```

### Trigger Conditions

- **Push**: Runs when code is pushed to main branches
- **Pull Request**: Runs when PRs are created/updated against main branches

## Step-by-Step Explanation

### Step 1: Checkout Repository

```yaml
- name: Checkout repository
  uses: actions/checkout@v4
```

**What it does**: Downloads your repository code to the runner
**Why needed**: The runner needs access to your source code

### Step 2: Cache APT Packages

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

**What it does**: 
- Caches package lists and metadata
- Speeds up subsequent runs by avoiding re-downloading packages

**Key components**:
- `path`: What files to cache
- `key`: Unique identifier for this cache
- `restore-keys`: Fallback keys if exact match not found

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

**What it does**:
- Installs required tools for C development
- Uses caching to speed up installation

**Tools installed**:
- `make`: Build automation tool
- `gcc`: C compiler
- `valgrind`: Memory debugging tool
- `cppcheck`: Static analysis tool
- `clang-format`: Code formatting tool
- `check`: Unit testing framework

**Optimizations**:
- `-qq`: Quiet mode (less output)
- `--no-install-recommends`: Skip recommended packages
- `2>/dev/null || true`: Suppress errors safely

### Step 4: Check for Source Files

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

**What it does**: Validates that source files exist before proceeding
**Why important**: Prevents CI from failing on empty repositories

### Step 5: Format and Check Code Style

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

**What it does**:
- Formats all C and H files using clang-format
- Checks if formatting changed anything (fails if code wasn't properly formatted)

**Configuration**: Uses `.clang-format` file in project root

### Step 6: Static Analysis (cppcheck)

```yaml
- name: Run static analysis (cppcheck)
  run: |
    echo "🧠 Running cppcheck..."
    cppcheck --enable=all --inconclusive --std=c11 --language=c \
      --force --check-config --suppress=missingIncludeSystem \
      --error-exitcode=1 -I include src tests
  shell: bash
```

**What it does**: Analyzes code for potential bugs and issues

**Flags explained**:
- `--enable=all`: Enable all checks
- `--inconclusive`: Show inconclusive results
- `--std=c11`: Use C11 standard
- `--language=c`: Specify C language
- `--force`: Force checking even with errors
- `--check-config`: Check configuration
- `--suppress=missingIncludeSystem`: Ignore missing system headers
- `--error-exitcode=1`: Exit with error if issues found
- `-I include`: Add include directory

### Step 7: Build Project

```yaml
- name: Build project
  run: |
    echo "🏗️ Building project..."
    make clean
    make build_if_available
  shell: bash
```

**What it does**:
- Cleans previous build artifacts
- Builds the project using Makefile

**Makefile targets**:
- `clean`: Remove build artifacts
- `build_if_available`: Build only if source files exist

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

**What it does**:
- Validates test files exist
- Runs unit tests using Check framework

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

**What it does**: Runs tests under Valgrind to detect memory leaks

**Valgrind flags**:
- `--leak-check=full`: Full memory leak detection
- `--error-exitcode=1`: Exit with error if leaks found

## Performance Optimizations

### 1. APT Package Caching

**Problem**: Installing packages every time is slow
**Solution**: Cache package lists and metadata

```yaml
- name: Cache apt packages
  uses: actions/cache@v3
  with:
    path: |
      /var/lib/apt/lists
      /var/cache/apt/pkgcache.bin
      /var/cache/apt/srcpkgcache.bin
```

**Benefits**:
- First run: Normal speed
- Subsequent runs: 50-75% faster

### 2. Permission Fix

**Problem**: Cache can't access apt directories
**Solution**: Make directories readable

```bash
sudo chmod -R a+r /var/lib/apt/lists/ 2>/dev/null || true
sudo chmod -R a+r /var/cache/apt/ 2>/dev/null || true
```

### 3. Smart Package Updates

**Problem**: Always running `apt-get update`
**Solution**: Only update on cache miss

```bash
if [ ! -f /var/cache/apt/pkgcache.bin ]; then
  sudo apt-get update -qq
else
  echo "✅ Using cached package lists..."
fi
```

## Troubleshooting

### Common Issues

#### 1. Permission Denied Errors

**Error**: `Cannot open: Permission denied`
**Solution**: Use the chmod approach in our workflow

#### 2. Package Not Found

**Error**: `Unable to locate package libcheck-dev`
**Solution**: Use correct package name `check`

#### 3. Valgrind Path Issues

**Error**: `No such file or directory`
**Solution**: Use relative paths instead of workspace paths

#### 4. Cache Not Working

**Symptoms**: Slow builds every time
**Solutions**:
- Check cache key uniqueness
- Verify paths are correct
- Ensure permissions are set

### Debug Tips

1. **Check logs**: Look at the Actions tab in GitHub
2. **Test locally**: Run commands manually
3. **Simplify**: Remove steps to isolate issues
4. **Use debug output**: Add `echo` statements

## Best Practices

### 1. Fail Fast

- Check prerequisites early
- Exit immediately on errors
- Don't waste time on invalid builds

### 2. Clear Error Messages

```bash
echo "❌ No source files found in src/ directory"
echo "   Source files are required for this project."
echo "   Please add source files to enable testing."
```

### 3. Use Meaningful Names

```yaml
- name: Format and check code style (clang-format)
- name: Run static analysis (cppcheck)
- name: Memory check (valgrind)
```

### 4. Optimize for Speed

- Cache dependencies
- Use parallel jobs when possible
- Skip unnecessary steps

### 5. Make It Reliable

- Handle edge cases
- Use proper error codes
- Test the workflow

## Advanced Features

### Matrix Builds

Test on multiple environments:

```yaml
strategy:
  matrix:
    os: [ubuntu-latest, ubuntu-20.04]
    compiler: [gcc, clang]
```

### Conditional Steps

Run steps only when needed:

```yaml
- name: Deploy
  if: github.ref == 'refs/heads/main'
  run: echo "Deploying to production"
```

### Artifacts

Save build outputs:

```yaml
- name: Upload build artifacts
  uses: actions/upload-artifact@v3
  with:
    name: build-output
    path: build/
```

## Conclusion

This CI/CD pipeline provides:

- ✅ **Automated code formatting**
- ✅ **Static analysis**
- ✅ **Build verification**
- ✅ **Unit testing**
- ✅ **Memory leak detection**
- ✅ **Performance optimization**

The pipeline ensures code quality and catches issues early, making development more efficient and reliable.

## Next Steps

1. **Set up branch protection rules** to require CI checks
2. **Add more test cases** to improve coverage
3. **Consider adding coverage reports**
4. **Implement automated releases** (CD)

For questions or improvements, refer to the GitHub Actions documentation or create an issue in the repository.
