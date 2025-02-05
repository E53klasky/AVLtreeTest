#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Create build directories if they do not exist
mkdir -p build_debug
mkdir -p build_release

# Build in Debug Mode
echo "Configuring Debug build..."
cmake -S . -B build_debug -DCMAKE_BUILD_TYPE=Debug

echo "Building Debug target..."
cmake --build build_debug

# uncomment when you think you have done it right for large test cases
# Build in Release Mode with -O3
#echo "Configuring Release build with optimization -O3..."
#cmake -S . -B build_release -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O3"

#echo "Building Release target..."
#cmake --build build_release

echo "Builds completed successfully!"
