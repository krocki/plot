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
include lib/nanogui/CMakeFiles/nanogui_python.dir/depend.make

# Include the progress variables for this target.
include lib/nanogui/CMakeFiles/nanogui_python.dir/progress.make

# Include the compile flags for this target's objects.
include lib/nanogui/CMakeFiles/nanogui_python.dir/flags.make

lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o: lib/nanogui/CMakeFiles/nanogui_python.dir/flags.make
lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o: ../lib/nanogui/python/python.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kmrocki/git/nanogui-test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o"
	cd /Users/kmrocki/git/nanogui-test/build/lib/nanogui && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/nanogui_python.dir/python/python.cpp.o -c /Users/kmrocki/git/nanogui-test/lib/nanogui/python/python.cpp

lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/nanogui_python.dir/python/python.cpp.i"
	cd /Users/kmrocki/git/nanogui-test/build/lib/nanogui && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kmrocki/git/nanogui-test/lib/nanogui/python/python.cpp > CMakeFiles/nanogui_python.dir/python/python.cpp.i

lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/nanogui_python.dir/python/python.cpp.s"
	cd /Users/kmrocki/git/nanogui-test/build/lib/nanogui && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kmrocki/git/nanogui-test/lib/nanogui/python/python.cpp -o CMakeFiles/nanogui_python.dir/python/python.cpp.s

lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o.requires:

.PHONY : lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o.requires

lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o.provides: lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o.requires
	$(MAKE) -f lib/nanogui/CMakeFiles/nanogui_python.dir/build.make lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o.provides.build
.PHONY : lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o.provides

lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o.provides.build: lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o


lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o: lib/nanogui/CMakeFiles/nanogui_python.dir/flags.make
lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o: ../lib/nanogui/ext/coro/coro.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kmrocki/git/nanogui-test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o"
	cd /Users/kmrocki/git/nanogui-test/build/lib/nanogui && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o   -c /Users/kmrocki/git/nanogui-test/lib/nanogui/ext/coro/coro.c

lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/nanogui_python.dir/ext/coro/coro.c.i"
	cd /Users/kmrocki/git/nanogui-test/build/lib/nanogui && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kmrocki/git/nanogui-test/lib/nanogui/ext/coro/coro.c > CMakeFiles/nanogui_python.dir/ext/coro/coro.c.i

lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/nanogui_python.dir/ext/coro/coro.c.s"
	cd /Users/kmrocki/git/nanogui-test/build/lib/nanogui && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kmrocki/git/nanogui-test/lib/nanogui/ext/coro/coro.c -o CMakeFiles/nanogui_python.dir/ext/coro/coro.c.s

lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o.requires:

.PHONY : lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o.requires

lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o.provides: lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o.requires
	$(MAKE) -f lib/nanogui/CMakeFiles/nanogui_python.dir/build.make lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o.provides.build
.PHONY : lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o.provides

lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o.provides.build: lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o


# Object files for target nanogui_python
nanogui_python_OBJECTS = \
"CMakeFiles/nanogui_python.dir/python/python.cpp.o" \
"CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o"

# External object files for target nanogui_python
nanogui_python_EXTERNAL_OBJECTS =

lib/nanogui/python/nanogui.so: lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o
lib/nanogui/python/nanogui.so: lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o
lib/nanogui/python/nanogui.so: lib/nanogui/CMakeFiles/nanogui_python.dir/build.make
lib/nanogui/python/nanogui.so: lib/nanogui/libnanogui.dylib
lib/nanogui/python/nanogui.so: lib/nanogui/CMakeFiles/nanogui_python.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kmrocki/git/nanogui-test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library python/nanogui.so"
	cd /Users/kmrocki/git/nanogui-test/build/lib/nanogui && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/nanogui_python.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/kmrocki/git/nanogui-test/build/lib/nanogui && strip -u -r /Users/kmrocki/git/nanogui-test/build/lib/nanogui/python/nanogui.so

# Rule to build all files generated by this target.
lib/nanogui/CMakeFiles/nanogui_python.dir/build: lib/nanogui/python/nanogui.so

.PHONY : lib/nanogui/CMakeFiles/nanogui_python.dir/build

lib/nanogui/CMakeFiles/nanogui_python.dir/requires: lib/nanogui/CMakeFiles/nanogui_python.dir/python/python.cpp.o.requires
lib/nanogui/CMakeFiles/nanogui_python.dir/requires: lib/nanogui/CMakeFiles/nanogui_python.dir/ext/coro/coro.c.o.requires

.PHONY : lib/nanogui/CMakeFiles/nanogui_python.dir/requires

lib/nanogui/CMakeFiles/nanogui_python.dir/clean:
	cd /Users/kmrocki/git/nanogui-test/build/lib/nanogui && $(CMAKE_COMMAND) -P CMakeFiles/nanogui_python.dir/cmake_clean.cmake
.PHONY : lib/nanogui/CMakeFiles/nanogui_python.dir/clean

lib/nanogui/CMakeFiles/nanogui_python.dir/depend:
	cd /Users/kmrocki/git/nanogui-test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kmrocki/git/nanogui-test /Users/kmrocki/git/nanogui-test/lib/nanogui /Users/kmrocki/git/nanogui-test/build /Users/kmrocki/git/nanogui-test/build/lib/nanogui /Users/kmrocki/git/nanogui-test/build/lib/nanogui/CMakeFiles/nanogui_python.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/nanogui/CMakeFiles/nanogui_python.dir/depend

