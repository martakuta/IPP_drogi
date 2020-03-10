# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /opt/clion-2018.3.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2018.3.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/marta/CLionProjects/IPP/drogi

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/marta/CLionProjects/IPP/drogi/cmake-build-debug

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/opt/clion-2018.3.4/bin/cmake/linux/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/opt/clion-2018.3.4/bin/cmake/linux/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/marta/CLionProjects/IPP/drogi/cmake-build-debug/CMakeFiles /home/marta/CLionProjects/IPP/drogi/cmake-build-debug/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/marta/CLionProjects/IPP/drogi/cmake-build-debug/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named map

# Build rule for target.
map: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 map
.PHONY : map

# fast build rule for target.
map/fast:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/build
.PHONY : map/fast

#=============================================================================
# Target rules for targets named doc

# Build rule for target.
doc: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 doc
.PHONY : doc

# fast build rule for target.
doc/fast:
	$(MAKE) -f CMakeFiles/doc.dir/build.make CMakeFiles/doc.dir/build
.PHONY : doc/fast

src/cities_and_roads.o: src/cities_and_roads.c.o

.PHONY : src/cities_and_roads.o

# target to build an object file
src/cities_and_roads.c.o:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/cities_and_roads.c.o
.PHONY : src/cities_and_roads.c.o

src/cities_and_roads.i: src/cities_and_roads.c.i

.PHONY : src/cities_and_roads.i

# target to preprocess a source file
src/cities_and_roads.c.i:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/cities_and_roads.c.i
.PHONY : src/cities_and_roads.c.i

src/cities_and_roads.s: src/cities_and_roads.c.s

.PHONY : src/cities_and_roads.s

# target to generate assembly for a file
src/cities_and_roads.c.s:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/cities_and_roads.c.s
.PHONY : src/cities_and_roads.c.s

src/map.o: src/map.c.o

.PHONY : src/map.o

# target to build an object file
src/map.c.o:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/map.c.o
.PHONY : src/map.c.o

src/map.i: src/map.c.i

.PHONY : src/map.i

# target to preprocess a source file
src/map.c.i:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/map.c.i
.PHONY : src/map.c.i

src/map.s: src/map.c.s

.PHONY : src/map.s

# target to generate assembly for a file
src/map.c.s:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/map.c.s
.PHONY : src/map.c.s

src/map_main.o: src/map_main.c.o

.PHONY : src/map_main.o

# target to build an object file
src/map_main.c.o:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/map_main.c.o
.PHONY : src/map_main.c.o

src/map_main.i: src/map_main.c.i

.PHONY : src/map_main.i

# target to preprocess a source file
src/map_main.c.i:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/map_main.c.i
.PHONY : src/map_main.c.i

src/map_main.s: src/map_main.c.s

.PHONY : src/map_main.s

# target to generate assembly for a file
src/map_main.c.s:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/map_main.c.s
.PHONY : src/map_main.c.s

src/reading.o: src/reading.c.o

.PHONY : src/reading.o

# target to build an object file
src/reading.c.o:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/reading.c.o
.PHONY : src/reading.c.o

src/reading.i: src/reading.c.i

.PHONY : src/reading.i

# target to preprocess a source file
src/reading.c.i:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/reading.c.i
.PHONY : src/reading.c.i

src/reading.s: src/reading.c.s

.PHONY : src/reading.s

# target to generate assembly for a file
src/reading.c.s:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/reading.c.s
.PHONY : src/reading.c.s

src/routes.o: src/routes.c.o

.PHONY : src/routes.o

# target to build an object file
src/routes.c.o:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/routes.c.o
.PHONY : src/routes.c.o

src/routes.i: src/routes.c.i

.PHONY : src/routes.i

# target to preprocess a source file
src/routes.c.i:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/routes.c.i
.PHONY : src/routes.c.i

src/routes.s: src/routes.c.s

.PHONY : src/routes.s

# target to generate assembly for a file
src/routes.c.s:
	$(MAKE) -f CMakeFiles/map.dir/build.make CMakeFiles/map.dir/src/routes.c.s
.PHONY : src/routes.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... map"
	@echo "... doc"
	@echo "... src/cities_and_roads.o"
	@echo "... src/cities_and_roads.i"
	@echo "... src/cities_and_roads.s"
	@echo "... src/map.o"
	@echo "... src/map.i"
	@echo "... src/map.s"
	@echo "... src/map_main.o"
	@echo "... src/map_main.i"
	@echo "... src/map_main.s"
	@echo "... src/reading.o"
	@echo "... src/reading.i"
	@echo "... src/reading.s"
	@echo "... src/routes.o"
	@echo "... src/routes.i"
	@echo "... src/routes.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
