# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass

# Include any dependencies generated for this target.
include InsertPass/CMakeFiles/ProfilePass.dir/depend.make

# Include the progress variables for this target.
include InsertPass/CMakeFiles/ProfilePass.dir/progress.make

# Include the compile flags for this target's objects.
include InsertPass/CMakeFiles/ProfilePass.dir/flags.make

InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o: InsertPass/CMakeFiles/ProfilePass.dir/flags.make
InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o: InsertPass/Pass.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o"
	cd /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/InsertPass && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ProfilePass.dir/Pass.cpp.o -c /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/InsertPass/Pass.cpp

InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ProfilePass.dir/Pass.cpp.i"
	cd /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/InsertPass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/InsertPass/Pass.cpp > CMakeFiles/ProfilePass.dir/Pass.cpp.i

InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ProfilePass.dir/Pass.cpp.s"
	cd /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/InsertPass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/InsertPass/Pass.cpp -o CMakeFiles/ProfilePass.dir/Pass.cpp.s

InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o.requires:

.PHONY : InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o.requires

InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o.provides: InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o.requires
	$(MAKE) -f InsertPass/CMakeFiles/ProfilePass.dir/build.make InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o.provides.build
.PHONY : InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o.provides

InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o.provides.build: InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o


# Object files for target ProfilePass
ProfilePass_OBJECTS = \
"CMakeFiles/ProfilePass.dir/Pass.cpp.o"

# External object files for target ProfilePass
ProfilePass_EXTERNAL_OBJECTS =

InsertPass/ProfilePass.so: InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o
InsertPass/ProfilePass.so: InsertPass/CMakeFiles/ProfilePass.dir/build.make
InsertPass/ProfilePass.so: InsertPass/CMakeFiles/ProfilePass.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module ProfilePass.so"
	cd /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/InsertPass && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ProfilePass.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
InsertPass/CMakeFiles/ProfilePass.dir/build: InsertPass/ProfilePass.so

.PHONY : InsertPass/CMakeFiles/ProfilePass.dir/build

InsertPass/CMakeFiles/ProfilePass.dir/requires: InsertPass/CMakeFiles/ProfilePass.dir/Pass.cpp.o.requires

.PHONY : InsertPass/CMakeFiles/ProfilePass.dir/requires

InsertPass/CMakeFiles/ProfilePass.dir/clean:
	cd /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/InsertPass && $(CMAKE_COMMAND) -P CMakeFiles/ProfilePass.dir/cmake_clean.cmake
.PHONY : InsertPass/CMakeFiles/ProfilePass.dir/clean

InsertPass/CMakeFiles/ProfilePass.dir/depend:
	cd /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/InsertPass /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/InsertPass /home/leo/Uni/praktikum/CSim-git/CSim/cache-sim/llvmPass/InsertPass/CMakeFiles/ProfilePass.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : InsertPass/CMakeFiles/ProfilePass.dir/depend
