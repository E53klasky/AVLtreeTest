#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Create build directories if they do not exist
mkdir -p build_debug
mkdir -p build_release

# Build in Debug Mode
echo -e "Configuring Debug build...\n"
cmake -S . -B build_debug -DCMAKE_BUILD_TYPE=Debug

echo -e " Building Debug target...\n"
cmake --build build_debug
cd build/
make
echo -e "Execuitbale made successfully!\n"

# g++ -std=c++14 -Werror -Wuninitialized -o build/test_io src/main.cpp  
# ./test_io only do this for seeing what the output should need not that helpuf for testing
# ./test_io 
echo -e "Running code...\n"
./Main
echo -e "Code run successfully !\n"

echo -e "Running test cases...\n"

./Tests
echo -e "Tests done running\n"


# uncomment when you think you have done it right for large test cases
# Build in Release Mode with -O3
#echo "Configuring Release build with optimization -O3..."
#cmake -S . -B build_release -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O3"

#echo "Building Release target..."
#cmake --build build_release

echo -e "Run completed\n"
