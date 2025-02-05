cmake_minimum_required(VERSION 3.22)
project(Project1)

set(CMAKE_CXX_STANDARD 14)

# Compile flags common to all configurations.
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror") # Remove -Wall if you don't want as many warnings treated as errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

# Set specific flags for different build types
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og") # Or you can keep it empty for minimal debug info
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

## Pull in desired version of Catch through CMake automatically, make it available
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

include_directories(src)

# Create the main executable
add_executable(Main
    src/main.cpp # your main file
)

# Create the test executable
add_executable(Tests
    test/test.cpp # your test file
)

# Link Catch2 to the test executable
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

# Enable testing
include(CTest)
include(Catch)
catch_discover_tests(Tests)