# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/qw/Documents/storage-node/tcp-client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/qw/Documents/storage-node/tcp-client/build

# Include any dependencies generated for this target.
include CMakeFiles/tcp-client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tcp-client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tcp-client.dir/flags.make

CMakeFiles/tcp-client.dir/main.o: CMakeFiles/tcp-client.dir/flags.make
CMakeFiles/tcp-client.dir/main.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qw/Documents/storage-node/tcp-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tcp-client.dir/main.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tcp-client.dir/main.o -c /home/qw/Documents/storage-node/tcp-client/main.cpp

CMakeFiles/tcp-client.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tcp-client.dir/main.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qw/Documents/storage-node/tcp-client/main.cpp > CMakeFiles/tcp-client.dir/main.i

CMakeFiles/tcp-client.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tcp-client.dir/main.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qw/Documents/storage-node/tcp-client/main.cpp -o CMakeFiles/tcp-client.dir/main.s

CMakeFiles/tcp-client.dir/main.o.requires:

.PHONY : CMakeFiles/tcp-client.dir/main.o.requires

CMakeFiles/tcp-client.dir/main.o.provides: CMakeFiles/tcp-client.dir/main.o.requires
	$(MAKE) -f CMakeFiles/tcp-client.dir/build.make CMakeFiles/tcp-client.dir/main.o.provides.build
.PHONY : CMakeFiles/tcp-client.dir/main.o.provides

CMakeFiles/tcp-client.dir/main.o.provides.build: CMakeFiles/tcp-client.dir/main.o


# Object files for target tcp-client
tcp__client_OBJECTS = \
"CMakeFiles/tcp-client.dir/main.o"

# External object files for target tcp-client
tcp__client_EXTERNAL_OBJECTS =

tcp-client: CMakeFiles/tcp-client.dir/main.o
tcp-client: CMakeFiles/tcp-client.dir/build.make
tcp-client: CMakeFiles/tcp-client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/qw/Documents/storage-node/tcp-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tcp-client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tcp-client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tcp-client.dir/build: tcp-client

.PHONY : CMakeFiles/tcp-client.dir/build

CMakeFiles/tcp-client.dir/requires: CMakeFiles/tcp-client.dir/main.o.requires

.PHONY : CMakeFiles/tcp-client.dir/requires

CMakeFiles/tcp-client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tcp-client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tcp-client.dir/clean

CMakeFiles/tcp-client.dir/depend:
	cd /home/qw/Documents/storage-node/tcp-client/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/qw/Documents/storage-node/tcp-client /home/qw/Documents/storage-node/tcp-client /home/qw/Documents/storage-node/tcp-client/build /home/qw/Documents/storage-node/tcp-client/build /home/qw/Documents/storage-node/tcp-client/build/CMakeFiles/tcp-client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tcp-client.dir/depend
