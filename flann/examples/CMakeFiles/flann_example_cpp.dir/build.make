# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Volumes/cmake-3.2.2-Darwin-x86_64/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Volumes/cmake-3.2.2-Darwin-x86_64/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Bryce/GitHub/flann

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Bryce/GitHub/flann/flann

# Include any dependencies generated for this target.
include examples/CMakeFiles/flann_example_cpp.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/flann_example_cpp.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/flann_example_cpp.dir/flags.make

examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o: examples/CMakeFiles/flann_example_cpp.dir/flags.make
examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o: ../examples/flann_example.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/Bryce/GitHub/flann/flann/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o"
	cd /Users/Bryce/GitHub/flann/flann/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o -c /Users/Bryce/GitHub/flann/examples/flann_example.cpp

examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/flann_example_cpp.dir/flann_example.cpp.i"
	cd /Users/Bryce/GitHub/flann/flann/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/Bryce/GitHub/flann/examples/flann_example.cpp > CMakeFiles/flann_example_cpp.dir/flann_example.cpp.i

examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/flann_example_cpp.dir/flann_example.cpp.s"
	cd /Users/Bryce/GitHub/flann/flann/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/Bryce/GitHub/flann/examples/flann_example.cpp -o CMakeFiles/flann_example_cpp.dir/flann_example.cpp.s

examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o.requires:
.PHONY : examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o.requires

examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o.provides: examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/flann_example_cpp.dir/build.make examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o.provides.build
.PHONY : examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o.provides

examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o.provides.build: examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o

# Object files for target flann_example_cpp
flann_example_cpp_OBJECTS = \
"CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o"

# External object files for target flann_example_cpp
flann_example_cpp_EXTERNAL_OBJECTS =

bin/flann_example_cpp: examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o
bin/flann_example_cpp: examples/CMakeFiles/flann_example_cpp.dir/build.make
bin/flann_example_cpp: /usr/local/hdf5/lib/libhdf5.dylib
bin/flann_example_cpp: /usr/lib/libz.dylib
bin/flann_example_cpp: /usr/lib/libm.dylib
bin/flann_example_cpp: lib/libflann_cpp.1.8.4.dylib
bin/flann_example_cpp: examples/CMakeFiles/flann_example_cpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/flann_example_cpp"
	cd /Users/Bryce/GitHub/flann/flann/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/flann_example_cpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/flann_example_cpp.dir/build: bin/flann_example_cpp
.PHONY : examples/CMakeFiles/flann_example_cpp.dir/build

examples/CMakeFiles/flann_example_cpp.dir/requires: examples/CMakeFiles/flann_example_cpp.dir/flann_example.cpp.o.requires
.PHONY : examples/CMakeFiles/flann_example_cpp.dir/requires

examples/CMakeFiles/flann_example_cpp.dir/clean:
	cd /Users/Bryce/GitHub/flann/flann/examples && $(CMAKE_COMMAND) -P CMakeFiles/flann_example_cpp.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/flann_example_cpp.dir/clean

examples/CMakeFiles/flann_example_cpp.dir/depend:
	cd /Users/Bryce/GitHub/flann/flann && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Bryce/GitHub/flann /Users/Bryce/GitHub/flann/examples /Users/Bryce/GitHub/flann/flann /Users/Bryce/GitHub/flann/flann/examples /Users/Bryce/GitHub/flann/flann/examples/CMakeFiles/flann_example_cpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/flann_example_cpp.dir/depend
