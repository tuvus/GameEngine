# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/oskar/iowastateuniversity/GameEngineProject/Game

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oskar/iowastateuniversity/GameEngineProject/Game/build

# Include any dependencies generated for this target.
include /home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include /home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/compiler_depend.make

# Include the progress variables for this target.
include /home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/progress.make

# Include the compile flags for this target's objects.
include /home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/flags.make

# Object files for target GameEngine
GameEngine_OBJECTS =

# External object files for target GameEngine
GameEngine_EXTERNAL_OBJECTS =

/home/oskar/iowastateuniversity/GameEngineProject/Game/lib/libGameEngine.a: /home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/build.make
/home/oskar/iowastateuniversity/GameEngineProject/Game/lib/libGameEngine.a: /home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/oskar/iowastateuniversity/GameEngineProject/Game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Linking CXX static library libGameEngine.a"
	cd /home/oskar/iowastateuniversity/GameEngineProject/Game/lib && $(CMAKE_COMMAND) -P CMakeFiles/GameEngine.dir/cmake_clean_target.cmake
	cd /home/oskar/iowastateuniversity/GameEngineProject/Game/lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GameEngine.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
/home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/build: /home/oskar/iowastateuniversity/GameEngineProject/Game/lib/libGameEngine.a
.PHONY : /home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/build

/home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/clean:
	cd /home/oskar/iowastateuniversity/GameEngineProject/Game/lib && $(CMAKE_COMMAND) -P CMakeFiles/GameEngine.dir/cmake_clean.cmake
.PHONY : /home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/clean

/home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/depend:
	cd /home/oskar/iowastateuniversity/GameEngineProject/Game/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oskar/iowastateuniversity/GameEngineProject/Game /home/oskar/iowastateuniversity/GameEngineProject/GameEngine /home/oskar/iowastateuniversity/GameEngineProject/Game/build /home/oskar/iowastateuniversity/GameEngineProject/Game/lib /home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : /home/oskar/iowastateuniversity/GameEngineProject/Game/lib/CMakeFiles/GameEngine.dir/depend

