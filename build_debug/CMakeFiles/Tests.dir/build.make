# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/adios/COP3530/AVLtreeTest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/adios/COP3530/AVLtreeTest/build_debug

# Include any dependencies generated for this target.
include CMakeFiles/Tests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Tests.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Tests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Tests.dir/flags.make

CMakeFiles/Tests.dir/test/test.cpp.o: CMakeFiles/Tests.dir/flags.make
CMakeFiles/Tests.dir/test/test.cpp.o: /home/adios/COP3530/AVLtreeTest/test/test.cpp
CMakeFiles/Tests.dir/test/test.cpp.o: CMakeFiles/Tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/adios/COP3530/AVLtreeTest/build_debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Tests.dir/test/test.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Tests.dir/test/test.cpp.o -MF CMakeFiles/Tests.dir/test/test.cpp.o.d -o CMakeFiles/Tests.dir/test/test.cpp.o -c /home/adios/COP3530/AVLtreeTest/test/test.cpp

CMakeFiles/Tests.dir/test/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Tests.dir/test/test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/adios/COP3530/AVLtreeTest/test/test.cpp > CMakeFiles/Tests.dir/test/test.cpp.i

CMakeFiles/Tests.dir/test/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Tests.dir/test/test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/adios/COP3530/AVLtreeTest/test/test.cpp -o CMakeFiles/Tests.dir/test/test.cpp.s

CMakeFiles/Tests.dir/src/AVLTree.cpp.o: CMakeFiles/Tests.dir/flags.make
CMakeFiles/Tests.dir/src/AVLTree.cpp.o: /home/adios/COP3530/AVLtreeTest/src/AVLTree.cpp
CMakeFiles/Tests.dir/src/AVLTree.cpp.o: CMakeFiles/Tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/adios/COP3530/AVLtreeTest/build_debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Tests.dir/src/AVLTree.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Tests.dir/src/AVLTree.cpp.o -MF CMakeFiles/Tests.dir/src/AVLTree.cpp.o.d -o CMakeFiles/Tests.dir/src/AVLTree.cpp.o -c /home/adios/COP3530/AVLtreeTest/src/AVLTree.cpp

CMakeFiles/Tests.dir/src/AVLTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Tests.dir/src/AVLTree.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/adios/COP3530/AVLtreeTest/src/AVLTree.cpp > CMakeFiles/Tests.dir/src/AVLTree.cpp.i

CMakeFiles/Tests.dir/src/AVLTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Tests.dir/src/AVLTree.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/adios/COP3530/AVLtreeTest/src/AVLTree.cpp -o CMakeFiles/Tests.dir/src/AVLTree.cpp.s

CMakeFiles/Tests.dir/src/AVLNODE.cpp.o: CMakeFiles/Tests.dir/flags.make
CMakeFiles/Tests.dir/src/AVLNODE.cpp.o: /home/adios/COP3530/AVLtreeTest/src/AVLNODE.cpp
CMakeFiles/Tests.dir/src/AVLNODE.cpp.o: CMakeFiles/Tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/adios/COP3530/AVLtreeTest/build_debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Tests.dir/src/AVLNODE.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Tests.dir/src/AVLNODE.cpp.o -MF CMakeFiles/Tests.dir/src/AVLNODE.cpp.o.d -o CMakeFiles/Tests.dir/src/AVLNODE.cpp.o -c /home/adios/COP3530/AVLtreeTest/src/AVLNODE.cpp

CMakeFiles/Tests.dir/src/AVLNODE.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Tests.dir/src/AVLNODE.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/adios/COP3530/AVLtreeTest/src/AVLNODE.cpp > CMakeFiles/Tests.dir/src/AVLNODE.cpp.i

CMakeFiles/Tests.dir/src/AVLNODE.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Tests.dir/src/AVLNODE.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/adios/COP3530/AVLtreeTest/src/AVLNODE.cpp -o CMakeFiles/Tests.dir/src/AVLNODE.cpp.s

CMakeFiles/Tests.dir/src/Student.cpp.o: CMakeFiles/Tests.dir/flags.make
CMakeFiles/Tests.dir/src/Student.cpp.o: /home/adios/COP3530/AVLtreeTest/src/Student.cpp
CMakeFiles/Tests.dir/src/Student.cpp.o: CMakeFiles/Tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/adios/COP3530/AVLtreeTest/build_debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Tests.dir/src/Student.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Tests.dir/src/Student.cpp.o -MF CMakeFiles/Tests.dir/src/Student.cpp.o.d -o CMakeFiles/Tests.dir/src/Student.cpp.o -c /home/adios/COP3530/AVLtreeTest/src/Student.cpp

CMakeFiles/Tests.dir/src/Student.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Tests.dir/src/Student.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/adios/COP3530/AVLtreeTest/src/Student.cpp > CMakeFiles/Tests.dir/src/Student.cpp.i

CMakeFiles/Tests.dir/src/Student.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Tests.dir/src/Student.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/adios/COP3530/AVLtreeTest/src/Student.cpp -o CMakeFiles/Tests.dir/src/Student.cpp.s

# Object files for target Tests
Tests_OBJECTS = \
"CMakeFiles/Tests.dir/test/test.cpp.o" \
"CMakeFiles/Tests.dir/src/AVLTree.cpp.o" \
"CMakeFiles/Tests.dir/src/AVLNODE.cpp.o" \
"CMakeFiles/Tests.dir/src/Student.cpp.o"

# External object files for target Tests
Tests_EXTERNAL_OBJECTS =

Tests: CMakeFiles/Tests.dir/test/test.cpp.o
Tests: CMakeFiles/Tests.dir/src/AVLTree.cpp.o
Tests: CMakeFiles/Tests.dir/src/AVLNODE.cpp.o
Tests: CMakeFiles/Tests.dir/src/Student.cpp.o
Tests: CMakeFiles/Tests.dir/build.make
Tests: _deps/catch2-build/src/libCatch2Maind.a
Tests: _deps/catch2-build/src/libCatch2d.a
Tests: CMakeFiles/Tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/adios/COP3530/AVLtreeTest/build_debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable Tests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Tests.dir/link.txt --verbose=$(VERBOSE)
	/usr/local/bin/cmake -D TEST_TARGET=Tests -D TEST_EXECUTABLE=/home/adios/COP3530/AVLtreeTest/build_debug/Tests -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/adios/COP3530/AVLtreeTest/build_debug -D TEST_SPEC= -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_LIST=Tests_TESTS -D TEST_REPORTER= -D TEST_OUTPUT_DIR= -D TEST_OUTPUT_PREFIX= -D TEST_OUTPUT_SUFFIX= -D TEST_DL_PATHS= -D CTEST_FILE=/home/adios/COP3530/AVLtreeTest/build_debug/Tests-b12d07c_tests.cmake -P /home/adios/COP3530/AVLtreeTest/build_debug/_deps/catch2-src/extras/CatchAddTests.cmake

# Rule to build all files generated by this target.
CMakeFiles/Tests.dir/build: Tests
.PHONY : CMakeFiles/Tests.dir/build

CMakeFiles/Tests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Tests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Tests.dir/clean

CMakeFiles/Tests.dir/depend:
	cd /home/adios/COP3530/AVLtreeTest/build_debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/adios/COP3530/AVLtreeTest /home/adios/COP3530/AVLtreeTest /home/adios/COP3530/AVLtreeTest/build_debug /home/adios/COP3530/AVLtreeTest/build_debug /home/adios/COP3530/AVLtreeTest/build_debug/CMakeFiles/Tests.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Tests.dir/depend

