# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/stathis/git/project_dit_emiris/project2/final

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/stathis/git/project_dit_emiris/project2/final

# Include any dependencies generated for this target.
include CMakeFiles/to_polygon.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/to_polygon.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/to_polygon.dir/flags.make

CMakeFiles/to_polygon.dir/convexHullAlgorithm.cpp.o: CMakeFiles/to_polygon.dir/flags.make
CMakeFiles/to_polygon.dir/convexHullAlgorithm.cpp.o: convexHullAlgorithm.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stathis/git/project_dit_emiris/project2/final/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/to_polygon.dir/convexHullAlgorithm.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/to_polygon.dir/convexHullAlgorithm.cpp.o -c /home/stathis/git/project_dit_emiris/project2/final/convexHullAlgorithm.cpp

CMakeFiles/to_polygon.dir/convexHullAlgorithm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/to_polygon.dir/convexHullAlgorithm.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stathis/git/project_dit_emiris/project2/final/convexHullAlgorithm.cpp > CMakeFiles/to_polygon.dir/convexHullAlgorithm.cpp.i

CMakeFiles/to_polygon.dir/convexHullAlgorithm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/to_polygon.dir/convexHullAlgorithm.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stathis/git/project_dit_emiris/project2/final/convexHullAlgorithm.cpp -o CMakeFiles/to_polygon.dir/convexHullAlgorithm.cpp.s

CMakeFiles/to_polygon.dir/incrementalAlgorithm.cpp.o: CMakeFiles/to_polygon.dir/flags.make
CMakeFiles/to_polygon.dir/incrementalAlgorithm.cpp.o: incrementalAlgorithm.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stathis/git/project_dit_emiris/project2/final/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/to_polygon.dir/incrementalAlgorithm.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/to_polygon.dir/incrementalAlgorithm.cpp.o -c /home/stathis/git/project_dit_emiris/project2/final/incrementalAlgorithm.cpp

CMakeFiles/to_polygon.dir/incrementalAlgorithm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/to_polygon.dir/incrementalAlgorithm.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stathis/git/project_dit_emiris/project2/final/incrementalAlgorithm.cpp > CMakeFiles/to_polygon.dir/incrementalAlgorithm.cpp.i

CMakeFiles/to_polygon.dir/incrementalAlgorithm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/to_polygon.dir/incrementalAlgorithm.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stathis/git/project_dit_emiris/project2/final/incrementalAlgorithm.cpp -o CMakeFiles/to_polygon.dir/incrementalAlgorithm.cpp.s

CMakeFiles/to_polygon.dir/main.cpp.o: CMakeFiles/to_polygon.dir/flags.make
CMakeFiles/to_polygon.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stathis/git/project_dit_emiris/project2/final/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/to_polygon.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/to_polygon.dir/main.cpp.o -c /home/stathis/git/project_dit_emiris/project2/final/main.cpp

CMakeFiles/to_polygon.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/to_polygon.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stathis/git/project_dit_emiris/project2/final/main.cpp > CMakeFiles/to_polygon.dir/main.cpp.i

CMakeFiles/to_polygon.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/to_polygon.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stathis/git/project_dit_emiris/project2/final/main.cpp -o CMakeFiles/to_polygon.dir/main.cpp.s

CMakeFiles/to_polygon.dir/sharedLib.cpp.o: CMakeFiles/to_polygon.dir/flags.make
CMakeFiles/to_polygon.dir/sharedLib.cpp.o: sharedLib.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stathis/git/project_dit_emiris/project2/final/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/to_polygon.dir/sharedLib.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/to_polygon.dir/sharedLib.cpp.o -c /home/stathis/git/project_dit_emiris/project2/final/sharedLib.cpp

CMakeFiles/to_polygon.dir/sharedLib.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/to_polygon.dir/sharedLib.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stathis/git/project_dit_emiris/project2/final/sharedLib.cpp > CMakeFiles/to_polygon.dir/sharedLib.cpp.i

CMakeFiles/to_polygon.dir/sharedLib.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/to_polygon.dir/sharedLib.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stathis/git/project_dit_emiris/project2/final/sharedLib.cpp -o CMakeFiles/to_polygon.dir/sharedLib.cpp.s

# Object files for target to_polygon
to_polygon_OBJECTS = \
"CMakeFiles/to_polygon.dir/convexHullAlgorithm.cpp.o" \
"CMakeFiles/to_polygon.dir/incrementalAlgorithm.cpp.o" \
"CMakeFiles/to_polygon.dir/main.cpp.o" \
"CMakeFiles/to_polygon.dir/sharedLib.cpp.o"

# External object files for target to_polygon
to_polygon_EXTERNAL_OBJECTS =

to_polygon: CMakeFiles/to_polygon.dir/convexHullAlgorithm.cpp.o
to_polygon: CMakeFiles/to_polygon.dir/incrementalAlgorithm.cpp.o
to_polygon: CMakeFiles/to_polygon.dir/main.cpp.o
to_polygon: CMakeFiles/to_polygon.dir/sharedLib.cpp.o
to_polygon: CMakeFiles/to_polygon.dir/build.make
to_polygon: /usr/lib/x86_64-linux-gnu/libmpfr.so
to_polygon: /usr/lib/x86_64-linux-gnu/libgmp.so
to_polygon: CMakeFiles/to_polygon.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/stathis/git/project_dit_emiris/project2/final/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable to_polygon"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/to_polygon.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/to_polygon.dir/build: to_polygon

.PHONY : CMakeFiles/to_polygon.dir/build

CMakeFiles/to_polygon.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/to_polygon.dir/cmake_clean.cmake
.PHONY : CMakeFiles/to_polygon.dir/clean

CMakeFiles/to_polygon.dir/depend:
	cd /home/stathis/git/project_dit_emiris/project2/final && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stathis/git/project_dit_emiris/project2/final /home/stathis/git/project_dit_emiris/project2/final /home/stathis/git/project_dit_emiris/project2/final /home/stathis/git/project_dit_emiris/project2/final /home/stathis/git/project_dit_emiris/project2/final/CMakeFiles/to_polygon.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/to_polygon.dir/depend

