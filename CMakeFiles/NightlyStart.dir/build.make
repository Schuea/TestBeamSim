# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = /nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/CMake/2.8.5/bin/cmake

# The command to remove a file.
RM = /nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/CMake/2.8.5/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/CMake/2.8.5/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /afs/desy.de/user/s/schuea/mymarlin

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /afs/desy.de/user/s/schuea/mymarlin

# Utility rule file for NightlyStart.

CMakeFiles/NightlyStart:
	/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/CMake/2.8.5/bin/ctest -D NightlyStart

NightlyStart: CMakeFiles/NightlyStart
NightlyStart: CMakeFiles/NightlyStart.dir/build.make
.PHONY : NightlyStart

# Rule to build all files generated by this target.
CMakeFiles/NightlyStart.dir/build: NightlyStart
.PHONY : CMakeFiles/NightlyStart.dir/build

CMakeFiles/NightlyStart.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/NightlyStart.dir/cmake_clean.cmake
.PHONY : CMakeFiles/NightlyStart.dir/clean

CMakeFiles/NightlyStart.dir/depend:
	cd /afs/desy.de/user/s/schuea/mymarlin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /afs/desy.de/user/s/schuea/mymarlin /afs/desy.de/user/s/schuea/mymarlin /afs/desy.de/user/s/schuea/mymarlin /afs/desy.de/user/s/schuea/mymarlin /afs/desy.de/user/s/schuea/mymarlin/CMakeFiles/NightlyStart.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/NightlyStart.dir/depend

