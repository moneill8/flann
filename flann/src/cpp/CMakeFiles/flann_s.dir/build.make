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
include src/cpp/CMakeFiles/flann_s.dir/depend.make

# Include the progress variables for this target.
include src/cpp/CMakeFiles/flann_s.dir/progress.make

# Include the compile flags for this target's objects.
include src/cpp/CMakeFiles/flann_s.dir/flags.make

src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o: src/cpp/CMakeFiles/flann_s.dir/flags.make
src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o: ../src/cpp/flann/flann.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/Bryce/GitHub/flann/flann/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o"
	cd /Users/Bryce/GitHub/flann/flann/src/cpp && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/flann_s.dir/flann/flann.cpp.o -c /Users/Bryce/GitHub/flann/src/cpp/flann/flann.cpp

src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/flann_s.dir/flann/flann.cpp.i"
	cd /Users/Bryce/GitHub/flann/flann/src/cpp && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/Bryce/GitHub/flann/src/cpp/flann/flann.cpp > CMakeFiles/flann_s.dir/flann/flann.cpp.i

src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/flann_s.dir/flann/flann.cpp.s"
	cd /Users/Bryce/GitHub/flann/flann/src/cpp && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/Bryce/GitHub/flann/src/cpp/flann/flann.cpp -o CMakeFiles/flann_s.dir/flann/flann.cpp.s

src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o.requires:
.PHONY : src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o.requires

src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o.provides: src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o.requires
	$(MAKE) -f src/cpp/CMakeFiles/flann_s.dir/build.make src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o.provides.build
.PHONY : src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o.provides

src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o.provides.build: src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o

src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o: src/cpp/CMakeFiles/flann_s.dir/flags.make
src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o: ../src/cpp/flann/ext/lz4.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/Bryce/GitHub/flann/flann/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o"
	cd /Users/Bryce/GitHub/flann/flann/src/cpp && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/flann_s.dir/flann/ext/lz4.c.o   -c /Users/Bryce/GitHub/flann/src/cpp/flann/ext/lz4.c

src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/flann_s.dir/flann/ext/lz4.c.i"
	cd /Users/Bryce/GitHub/flann/flann/src/cpp && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/Bryce/GitHub/flann/src/cpp/flann/ext/lz4.c > CMakeFiles/flann_s.dir/flann/ext/lz4.c.i

src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/flann_s.dir/flann/ext/lz4.c.s"
	cd /Users/Bryce/GitHub/flann/flann/src/cpp && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/Bryce/GitHub/flann/src/cpp/flann/ext/lz4.c -o CMakeFiles/flann_s.dir/flann/ext/lz4.c.s

src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o.requires:
.PHONY : src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o.requires

src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o.provides: src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o.requires
	$(MAKE) -f src/cpp/CMakeFiles/flann_s.dir/build.make src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o.provides.build
.PHONY : src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o.provides

src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o.provides.build: src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o

src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o: src/cpp/CMakeFiles/flann_s.dir/flags.make
src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o: ../src/cpp/flann/ext/lz4hc.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/Bryce/GitHub/flann/flann/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o"
	cd /Users/Bryce/GitHub/flann/flann/src/cpp && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o   -c /Users/Bryce/GitHub/flann/src/cpp/flann/ext/lz4hc.c

src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.i"
	cd /Users/Bryce/GitHub/flann/flann/src/cpp && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/Bryce/GitHub/flann/src/cpp/flann/ext/lz4hc.c > CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.i

src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.s"
	cd /Users/Bryce/GitHub/flann/flann/src/cpp && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/Bryce/GitHub/flann/src/cpp/flann/ext/lz4hc.c -o CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.s

src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o.requires:
.PHONY : src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o.requires

src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o.provides: src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o.requires
	$(MAKE) -f src/cpp/CMakeFiles/flann_s.dir/build.make src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o.provides.build
.PHONY : src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o.provides

src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o.provides.build: src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o

# Object files for target flann_s
flann_s_OBJECTS = \
"CMakeFiles/flann_s.dir/flann/flann.cpp.o" \
"CMakeFiles/flann_s.dir/flann/ext/lz4.c.o" \
"CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o"

# External object files for target flann_s
flann_s_EXTERNAL_OBJECTS =

lib/libflann_s.a: src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o
lib/libflann_s.a: src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o
lib/libflann_s.a: src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o
lib/libflann_s.a: src/cpp/CMakeFiles/flann_s.dir/build.make
lib/libflann_s.a: src/cpp/CMakeFiles/flann_s.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../../lib/libflann_s.a"
	cd /Users/Bryce/GitHub/flann/flann/src/cpp && $(CMAKE_COMMAND) -P CMakeFiles/flann_s.dir/cmake_clean_target.cmake
	cd /Users/Bryce/GitHub/flann/flann/src/cpp && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/flann_s.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/cpp/CMakeFiles/flann_s.dir/build: lib/libflann_s.a
.PHONY : src/cpp/CMakeFiles/flann_s.dir/build

src/cpp/CMakeFiles/flann_s.dir/requires: src/cpp/CMakeFiles/flann_s.dir/flann/flann.cpp.o.requires
src/cpp/CMakeFiles/flann_s.dir/requires: src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4.c.o.requires
src/cpp/CMakeFiles/flann_s.dir/requires: src/cpp/CMakeFiles/flann_s.dir/flann/ext/lz4hc.c.o.requires
.PHONY : src/cpp/CMakeFiles/flann_s.dir/requires

src/cpp/CMakeFiles/flann_s.dir/clean:
	cd /Users/Bryce/GitHub/flann/flann/src/cpp && $(CMAKE_COMMAND) -P CMakeFiles/flann_s.dir/cmake_clean.cmake
.PHONY : src/cpp/CMakeFiles/flann_s.dir/clean

src/cpp/CMakeFiles/flann_s.dir/depend:
	cd /Users/Bryce/GitHub/flann/flann && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Bryce/GitHub/flann /Users/Bryce/GitHub/flann/src/cpp /Users/Bryce/GitHub/flann/flann /Users/Bryce/GitHub/flann/flann/src/cpp /Users/Bryce/GitHub/flann/flann/src/cpp/CMakeFiles/flann_s.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/cpp/CMakeFiles/flann_s.dir/depend
