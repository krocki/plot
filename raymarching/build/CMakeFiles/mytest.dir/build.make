# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.7.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.7.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kmrocki/git/nanogui-test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kmrocki/git/nanogui-test/build

# Include any dependencies generated for this target.
include CMakeFiles/mytest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mytest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mytest.dir/flags.make

shader/vert.glsl.c: ../shader/vert.glsl
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/kmrocki/git/nanogui-test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building binary file for embedding /Users/kmrocki/git/nanogui-test/build/shader/vert.glsl.c"
	cd /Users/kmrocki/git/nanogui-test && /Users/kmrocki/git/nanogui-test/build/lib/embed-resource/embed-resource /Users/kmrocki/git/nanogui-test/build/shader/vert.glsl.c shader/vert.glsl

shader/frag.glsl.c: ../shader/frag.glsl
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/kmrocki/git/nanogui-test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building binary file for embedding /Users/kmrocki/git/nanogui-test/build/shader/frag.glsl.c"
	cd /Users/kmrocki/git/nanogui-test && /Users/kmrocki/git/nanogui-test/build/lib/embed-resource/embed-resource /Users/kmrocki/git/nanogui-test/build/shader/frag.glsl.c shader/frag.glsl

CMakeFiles/mytest.dir/test.cpp.o: CMakeFiles/mytest.dir/flags.make
CMakeFiles/mytest.dir/test.cpp.o: ../test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kmrocki/git/nanogui-test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/mytest.dir/test.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mytest.dir/test.cpp.o -c /Users/kmrocki/git/nanogui-test/test.cpp

CMakeFiles/mytest.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mytest.dir/test.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kmrocki/git/nanogui-test/test.cpp > CMakeFiles/mytest.dir/test.cpp.i

CMakeFiles/mytest.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mytest.dir/test.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kmrocki/git/nanogui-test/test.cpp -o CMakeFiles/mytest.dir/test.cpp.s

CMakeFiles/mytest.dir/test.cpp.o.requires:

.PHONY : CMakeFiles/mytest.dir/test.cpp.o.requires

CMakeFiles/mytest.dir/test.cpp.o.provides: CMakeFiles/mytest.dir/test.cpp.o.requires
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/test.cpp.o.provides.build
.PHONY : CMakeFiles/mytest.dir/test.cpp.o.provides

CMakeFiles/mytest.dir/test.cpp.o.provides.build: CMakeFiles/mytest.dir/test.cpp.o


CMakeFiles/mytest.dir/shader/vert.glsl.c.o: CMakeFiles/mytest.dir/flags.make
CMakeFiles/mytest.dir/shader/vert.glsl.c.o: shader/vert.glsl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kmrocki/git/nanogui-test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/mytest.dir/shader/vert.glsl.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mytest.dir/shader/vert.glsl.c.o   -c /Users/kmrocki/git/nanogui-test/build/shader/vert.glsl.c

CMakeFiles/mytest.dir/shader/vert.glsl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mytest.dir/shader/vert.glsl.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kmrocki/git/nanogui-test/build/shader/vert.glsl.c > CMakeFiles/mytest.dir/shader/vert.glsl.c.i

CMakeFiles/mytest.dir/shader/vert.glsl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mytest.dir/shader/vert.glsl.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kmrocki/git/nanogui-test/build/shader/vert.glsl.c -o CMakeFiles/mytest.dir/shader/vert.glsl.c.s

CMakeFiles/mytest.dir/shader/vert.glsl.c.o.requires:

.PHONY : CMakeFiles/mytest.dir/shader/vert.glsl.c.o.requires

CMakeFiles/mytest.dir/shader/vert.glsl.c.o.provides: CMakeFiles/mytest.dir/shader/vert.glsl.c.o.requires
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/shader/vert.glsl.c.o.provides.build
.PHONY : CMakeFiles/mytest.dir/shader/vert.glsl.c.o.provides

CMakeFiles/mytest.dir/shader/vert.glsl.c.o.provides.build: CMakeFiles/mytest.dir/shader/vert.glsl.c.o


CMakeFiles/mytest.dir/shader/frag.glsl.c.o: CMakeFiles/mytest.dir/flags.make
CMakeFiles/mytest.dir/shader/frag.glsl.c.o: shader/frag.glsl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kmrocki/git/nanogui-test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/mytest.dir/shader/frag.glsl.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mytest.dir/shader/frag.glsl.c.o   -c /Users/kmrocki/git/nanogui-test/build/shader/frag.glsl.c

CMakeFiles/mytest.dir/shader/frag.glsl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mytest.dir/shader/frag.glsl.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kmrocki/git/nanogui-test/build/shader/frag.glsl.c > CMakeFiles/mytest.dir/shader/frag.glsl.c.i

CMakeFiles/mytest.dir/shader/frag.glsl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mytest.dir/shader/frag.glsl.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kmrocki/git/nanogui-test/build/shader/frag.glsl.c -o CMakeFiles/mytest.dir/shader/frag.glsl.c.s

CMakeFiles/mytest.dir/shader/frag.glsl.c.o.requires:

.PHONY : CMakeFiles/mytest.dir/shader/frag.glsl.c.o.requires

CMakeFiles/mytest.dir/shader/frag.glsl.c.o.provides: CMakeFiles/mytest.dir/shader/frag.glsl.c.o.requires
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/shader/frag.glsl.c.o.provides.build
.PHONY : CMakeFiles/mytest.dir/shader/frag.glsl.c.o.provides

CMakeFiles/mytest.dir/shader/frag.glsl.c.o.provides.build: CMakeFiles/mytest.dir/shader/frag.glsl.c.o


# Object files for target mytest
mytest_OBJECTS = \
"CMakeFiles/mytest.dir/test.cpp.o" \
"CMakeFiles/mytest.dir/shader/vert.glsl.c.o" \
"CMakeFiles/mytest.dir/shader/frag.glsl.c.o"

# External object files for target mytest
mytest_EXTERNAL_OBJECTS =

mytest: CMakeFiles/mytest.dir/test.cpp.o
mytest: CMakeFiles/mytest.dir/shader/vert.glsl.c.o
mytest: CMakeFiles/mytest.dir/shader/frag.glsl.c.o
mytest: CMakeFiles/mytest.dir/build.make
mytest: lib/nanogui/libnanogui.dylib
mytest: CMakeFiles/mytest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kmrocki/git/nanogui-test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable mytest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mytest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mytest.dir/build: mytest

.PHONY : CMakeFiles/mytest.dir/build

CMakeFiles/mytest.dir/requires: CMakeFiles/mytest.dir/test.cpp.o.requires
CMakeFiles/mytest.dir/requires: CMakeFiles/mytest.dir/shader/vert.glsl.c.o.requires
CMakeFiles/mytest.dir/requires: CMakeFiles/mytest.dir/shader/frag.glsl.c.o.requires

.PHONY : CMakeFiles/mytest.dir/requires

CMakeFiles/mytest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mytest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mytest.dir/clean

CMakeFiles/mytest.dir/depend: shader/vert.glsl.c
CMakeFiles/mytest.dir/depend: shader/frag.glsl.c
	cd /Users/kmrocki/git/nanogui-test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kmrocki/git/nanogui-test /Users/kmrocki/git/nanogui-test /Users/kmrocki/git/nanogui-test/build /Users/kmrocki/git/nanogui-test/build /Users/kmrocki/git/nanogui-test/build/CMakeFiles/mytest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mytest.dir/depend

