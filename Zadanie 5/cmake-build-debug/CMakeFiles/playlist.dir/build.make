# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/cgev/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/193.5662.56/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/cgev/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/193.5662.56/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/cgev/Desktop/CS/C++/JNP/Zadanie 5"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/cgev/Desktop/CS/C++/JNP/Zadanie 5/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/playlist.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/playlist.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/playlist.dir/flags.make

CMakeFiles/playlist.dir/insertion_ordered_map_example.cc.o: CMakeFiles/playlist.dir/flags.make
CMakeFiles/playlist.dir/insertion_ordered_map_example.cc.o: ../insertion_ordered_map_example.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/cgev/Desktop/CS/C++/JNP/Zadanie 5/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/playlist.dir/insertion_ordered_map_example.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/playlist.dir/insertion_ordered_map_example.cc.o -c "/home/cgev/Desktop/CS/C++/JNP/Zadanie 5/insertion_ordered_map_example.cc"

CMakeFiles/playlist.dir/insertion_ordered_map_example.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/playlist.dir/insertion_ordered_map_example.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/cgev/Desktop/CS/C++/JNP/Zadanie 5/insertion_ordered_map_example.cc" > CMakeFiles/playlist.dir/insertion_ordered_map_example.cc.i

CMakeFiles/playlist.dir/insertion_ordered_map_example.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/playlist.dir/insertion_ordered_map_example.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/cgev/Desktop/CS/C++/JNP/Zadanie 5/insertion_ordered_map_example.cc" -o CMakeFiles/playlist.dir/insertion_ordered_map_example.cc.s

# Object files for target playlist
playlist_OBJECTS = \
"CMakeFiles/playlist.dir/insertion_ordered_map_example.cc.o"

# External object files for target playlist
playlist_EXTERNAL_OBJECTS =

playlist: CMakeFiles/playlist.dir/insertion_ordered_map_example.cc.o
playlist: CMakeFiles/playlist.dir/build.make
playlist: CMakeFiles/playlist.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/cgev/Desktop/CS/C++/JNP/Zadanie 5/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable playlist"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/playlist.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/playlist.dir/build: playlist

.PHONY : CMakeFiles/playlist.dir/build

CMakeFiles/playlist.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/playlist.dir/cmake_clean.cmake
.PHONY : CMakeFiles/playlist.dir/clean

CMakeFiles/playlist.dir/depend:
	cd "/home/cgev/Desktop/CS/C++/JNP/Zadanie 5/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/cgev/Desktop/CS/C++/JNP/Zadanie 5" "/home/cgev/Desktop/CS/C++/JNP/Zadanie 5" "/home/cgev/Desktop/CS/C++/JNP/Zadanie 5/cmake-build-debug" "/home/cgev/Desktop/CS/C++/JNP/Zadanie 5/cmake-build-debug" "/home/cgev/Desktop/CS/C++/JNP/Zadanie 5/cmake-build-debug/CMakeFiles/playlist.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/playlist.dir/depend

