#!/bin/bash

# Auto-format script for s21_matrix project
# This script formats all C and H files using clang-format

echo "🔧 Auto-formatting code before push..."

# Check if clang-format is available
if ! command -v clang-format &> /dev/null; then
    echo "❌ clang-format not found. Please install it:"
    echo "   Ubuntu/Debian: sudo apt-get install clang-format"
    echo "   macOS: brew install clang-format"
    exit 1
fi

# Find and format all C and H files
echo "📝 Formatting source files..."
find src include tests -name '*.c' -o -name '*.h' 2>/dev/null | while read -r file; do
    if [ -f "$file" ]; then
        echo "  Formatting: $file"
        clang-format -i "$file"
    fi
done

# Check if any files were changed
if ! git diff --quiet; then
    echo "✅ Code has been auto-formatted. Changes:"
    git diff --name-only
    echo ""
    echo "📋 Formatted files have been staged. You can review changes with:"
    echo "   git diff --cached"
    echo ""
    echo "🚀 Ready to push!"
else
    echo "✅ Code is already properly formatted"
fi

echo "🎉 Auto-formatting complete!"
