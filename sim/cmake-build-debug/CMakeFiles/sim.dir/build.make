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
CMAKE_COMMAND = "/Users/chenxuehai/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/181.5281.33/CLion.app/Contents/bin/cmake/bin/cmake"

# The command to remove a file.
RM = "/Users/chenxuehai/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/181.5281.33/CLion.app/Contents/bin/cmake/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/chenxuehai/Documents/GitHub/CSAPP/sim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/sim.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sim.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sim.dir/flags.make

CMakeFiles/sim.dir/misc/examples.c.o: CMakeFiles/sim.dir/flags.make
CMakeFiles/sim.dir/misc/examples.c.o: ../misc/examples.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/sim.dir/misc/examples.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sim.dir/misc/examples.c.o   -c /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/examples.c

CMakeFiles/sim.dir/misc/examples.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sim.dir/misc/examples.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/examples.c > CMakeFiles/sim.dir/misc/examples.c.i

CMakeFiles/sim.dir/misc/examples.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sim.dir/misc/examples.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/examples.c -o CMakeFiles/sim.dir/misc/examples.c.s

CMakeFiles/sim.dir/misc/examples.c.o.requires:

.PHONY : CMakeFiles/sim.dir/misc/examples.c.o.requires

CMakeFiles/sim.dir/misc/examples.c.o.provides: CMakeFiles/sim.dir/misc/examples.c.o.requires
	$(MAKE) -f CMakeFiles/sim.dir/build.make CMakeFiles/sim.dir/misc/examples.c.o.provides.build
.PHONY : CMakeFiles/sim.dir/misc/examples.c.o.provides

CMakeFiles/sim.dir/misc/examples.c.o.provides.build: CMakeFiles/sim.dir/misc/examples.c.o


CMakeFiles/sim.dir/misc/isa.c.o: CMakeFiles/sim.dir/flags.make
CMakeFiles/sim.dir/misc/isa.c.o: ../misc/isa.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/sim.dir/misc/isa.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sim.dir/misc/isa.c.o   -c /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/isa.c

CMakeFiles/sim.dir/misc/isa.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sim.dir/misc/isa.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/isa.c > CMakeFiles/sim.dir/misc/isa.c.i

CMakeFiles/sim.dir/misc/isa.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sim.dir/misc/isa.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/isa.c -o CMakeFiles/sim.dir/misc/isa.c.s

CMakeFiles/sim.dir/misc/isa.c.o.requires:

.PHONY : CMakeFiles/sim.dir/misc/isa.c.o.requires

CMakeFiles/sim.dir/misc/isa.c.o.provides: CMakeFiles/sim.dir/misc/isa.c.o.requires
	$(MAKE) -f CMakeFiles/sim.dir/build.make CMakeFiles/sim.dir/misc/isa.c.o.provides.build
.PHONY : CMakeFiles/sim.dir/misc/isa.c.o.provides

CMakeFiles/sim.dir/misc/isa.c.o.provides.build: CMakeFiles/sim.dir/misc/isa.c.o


CMakeFiles/sim.dir/misc/node.c.o: CMakeFiles/sim.dir/flags.make
CMakeFiles/sim.dir/misc/node.c.o: ../misc/node.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/sim.dir/misc/node.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sim.dir/misc/node.c.o   -c /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/node.c

CMakeFiles/sim.dir/misc/node.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sim.dir/misc/node.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/node.c > CMakeFiles/sim.dir/misc/node.c.i

CMakeFiles/sim.dir/misc/node.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sim.dir/misc/node.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/node.c -o CMakeFiles/sim.dir/misc/node.c.s

CMakeFiles/sim.dir/misc/node.c.o.requires:

.PHONY : CMakeFiles/sim.dir/misc/node.c.o.requires

CMakeFiles/sim.dir/misc/node.c.o.provides: CMakeFiles/sim.dir/misc/node.c.o.requires
	$(MAKE) -f CMakeFiles/sim.dir/build.make CMakeFiles/sim.dir/misc/node.c.o.provides.build
.PHONY : CMakeFiles/sim.dir/misc/node.c.o.provides

CMakeFiles/sim.dir/misc/node.c.o.provides.build: CMakeFiles/sim.dir/misc/node.c.o


CMakeFiles/sim.dir/misc/outgen.c.o: CMakeFiles/sim.dir/flags.make
CMakeFiles/sim.dir/misc/outgen.c.o: ../misc/outgen.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/sim.dir/misc/outgen.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sim.dir/misc/outgen.c.o   -c /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/outgen.c

CMakeFiles/sim.dir/misc/outgen.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sim.dir/misc/outgen.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/outgen.c > CMakeFiles/sim.dir/misc/outgen.c.i

CMakeFiles/sim.dir/misc/outgen.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sim.dir/misc/outgen.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/outgen.c -o CMakeFiles/sim.dir/misc/outgen.c.s

CMakeFiles/sim.dir/misc/outgen.c.o.requires:

.PHONY : CMakeFiles/sim.dir/misc/outgen.c.o.requires

CMakeFiles/sim.dir/misc/outgen.c.o.provides: CMakeFiles/sim.dir/misc/outgen.c.o.requires
	$(MAKE) -f CMakeFiles/sim.dir/build.make CMakeFiles/sim.dir/misc/outgen.c.o.provides.build
.PHONY : CMakeFiles/sim.dir/misc/outgen.c.o.provides

CMakeFiles/sim.dir/misc/outgen.c.o.provides.build: CMakeFiles/sim.dir/misc/outgen.c.o


CMakeFiles/sim.dir/misc/yas-grammar.c.o: CMakeFiles/sim.dir/flags.make
CMakeFiles/sim.dir/misc/yas-grammar.c.o: ../misc/yas-grammar.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/sim.dir/misc/yas-grammar.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sim.dir/misc/yas-grammar.c.o   -c /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/yas-grammar.c

CMakeFiles/sim.dir/misc/yas-grammar.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sim.dir/misc/yas-grammar.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/yas-grammar.c > CMakeFiles/sim.dir/misc/yas-grammar.c.i

CMakeFiles/sim.dir/misc/yas-grammar.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sim.dir/misc/yas-grammar.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/yas-grammar.c -o CMakeFiles/sim.dir/misc/yas-grammar.c.s

CMakeFiles/sim.dir/misc/yas-grammar.c.o.requires:

.PHONY : CMakeFiles/sim.dir/misc/yas-grammar.c.o.requires

CMakeFiles/sim.dir/misc/yas-grammar.c.o.provides: CMakeFiles/sim.dir/misc/yas-grammar.c.o.requires
	$(MAKE) -f CMakeFiles/sim.dir/build.make CMakeFiles/sim.dir/misc/yas-grammar.c.o.provides.build
.PHONY : CMakeFiles/sim.dir/misc/yas-grammar.c.o.provides

CMakeFiles/sim.dir/misc/yas-grammar.c.o.provides.build: CMakeFiles/sim.dir/misc/yas-grammar.c.o


CMakeFiles/sim.dir/misc/yas.c.o: CMakeFiles/sim.dir/flags.make
CMakeFiles/sim.dir/misc/yas.c.o: ../misc/yas.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/sim.dir/misc/yas.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sim.dir/misc/yas.c.o   -c /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/yas.c

CMakeFiles/sim.dir/misc/yas.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sim.dir/misc/yas.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/yas.c > CMakeFiles/sim.dir/misc/yas.c.i

CMakeFiles/sim.dir/misc/yas.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sim.dir/misc/yas.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/yas.c -o CMakeFiles/sim.dir/misc/yas.c.s

CMakeFiles/sim.dir/misc/yas.c.o.requires:

.PHONY : CMakeFiles/sim.dir/misc/yas.c.o.requires

CMakeFiles/sim.dir/misc/yas.c.o.provides: CMakeFiles/sim.dir/misc/yas.c.o.requires
	$(MAKE) -f CMakeFiles/sim.dir/build.make CMakeFiles/sim.dir/misc/yas.c.o.provides.build
.PHONY : CMakeFiles/sim.dir/misc/yas.c.o.provides

CMakeFiles/sim.dir/misc/yas.c.o.provides.build: CMakeFiles/sim.dir/misc/yas.c.o


CMakeFiles/sim.dir/misc/yis.c.o: CMakeFiles/sim.dir/flags.make
CMakeFiles/sim.dir/misc/yis.c.o: ../misc/yis.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/sim.dir/misc/yis.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sim.dir/misc/yis.c.o   -c /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/yis.c

CMakeFiles/sim.dir/misc/yis.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sim.dir/misc/yis.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/yis.c > CMakeFiles/sim.dir/misc/yis.c.i

CMakeFiles/sim.dir/misc/yis.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sim.dir/misc/yis.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/yis.c -o CMakeFiles/sim.dir/misc/yis.c.s

CMakeFiles/sim.dir/misc/yis.c.o.requires:

.PHONY : CMakeFiles/sim.dir/misc/yis.c.o.requires

CMakeFiles/sim.dir/misc/yis.c.o.provides: CMakeFiles/sim.dir/misc/yis.c.o.requires
	$(MAKE) -f CMakeFiles/sim.dir/build.make CMakeFiles/sim.dir/misc/yis.c.o.provides.build
.PHONY : CMakeFiles/sim.dir/misc/yis.c.o.provides

CMakeFiles/sim.dir/misc/yis.c.o.provides.build: CMakeFiles/sim.dir/misc/yis.c.o


CMakeFiles/sim.dir/pipe/ncopy.c.o: CMakeFiles/sim.dir/flags.make
CMakeFiles/sim.dir/pipe/ncopy.c.o: ../pipe/ncopy.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/sim.dir/pipe/ncopy.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sim.dir/pipe/ncopy.c.o   -c /Users/chenxuehai/Documents/GitHub/CSAPP/sim/pipe/ncopy.c

CMakeFiles/sim.dir/pipe/ncopy.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sim.dir/pipe/ncopy.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/chenxuehai/Documents/GitHub/CSAPP/sim/pipe/ncopy.c > CMakeFiles/sim.dir/pipe/ncopy.c.i

CMakeFiles/sim.dir/pipe/ncopy.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sim.dir/pipe/ncopy.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/chenxuehai/Documents/GitHub/CSAPP/sim/pipe/ncopy.c -o CMakeFiles/sim.dir/pipe/ncopy.c.s

CMakeFiles/sim.dir/pipe/ncopy.c.o.requires:

.PHONY : CMakeFiles/sim.dir/pipe/ncopy.c.o.requires

CMakeFiles/sim.dir/pipe/ncopy.c.o.provides: CMakeFiles/sim.dir/pipe/ncopy.c.o.requires
	$(MAKE) -f CMakeFiles/sim.dir/build.make CMakeFiles/sim.dir/pipe/ncopy.c.o.provides.build
.PHONY : CMakeFiles/sim.dir/pipe/ncopy.c.o.provides

CMakeFiles/sim.dir/pipe/ncopy.c.o.provides.build: CMakeFiles/sim.dir/pipe/ncopy.c.o


CMakeFiles/sim.dir/pipe/psim.c.o: CMakeFiles/sim.dir/flags.make
CMakeFiles/sim.dir/pipe/psim.c.o: ../pipe/psim.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/sim.dir/pipe/psim.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sim.dir/pipe/psim.c.o   -c /Users/chenxuehai/Documents/GitHub/CSAPP/sim/pipe/psim.c

CMakeFiles/sim.dir/pipe/psim.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sim.dir/pipe/psim.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/chenxuehai/Documents/GitHub/CSAPP/sim/pipe/psim.c > CMakeFiles/sim.dir/pipe/psim.c.i

CMakeFiles/sim.dir/pipe/psim.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sim.dir/pipe/psim.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/chenxuehai/Documents/GitHub/CSAPP/sim/pipe/psim.c -o CMakeFiles/sim.dir/pipe/psim.c.s

CMakeFiles/sim.dir/pipe/psim.c.o.requires:

.PHONY : CMakeFiles/sim.dir/pipe/psim.c.o.requires

CMakeFiles/sim.dir/pipe/psim.c.o.provides: CMakeFiles/sim.dir/pipe/psim.c.o.requires
	$(MAKE) -f CMakeFiles/sim.dir/build.make CMakeFiles/sim.dir/pipe/psim.c.o.provides.build
.PHONY : CMakeFiles/sim.dir/pipe/psim.c.o.provides

CMakeFiles/sim.dir/pipe/psim.c.o.provides.build: CMakeFiles/sim.dir/pipe/psim.c.o


CMakeFiles/sim.dir/seq/ssim.c.o: CMakeFiles/sim.dir/flags.make
CMakeFiles/sim.dir/seq/ssim.c.o: ../seq/ssim.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/sim.dir/seq/ssim.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sim.dir/seq/ssim.c.o   -c /Users/chenxuehai/Documents/GitHub/CSAPP/sim/seq/ssim.c

CMakeFiles/sim.dir/seq/ssim.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sim.dir/seq/ssim.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/chenxuehai/Documents/GitHub/CSAPP/sim/seq/ssim.c > CMakeFiles/sim.dir/seq/ssim.c.i

CMakeFiles/sim.dir/seq/ssim.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sim.dir/seq/ssim.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/chenxuehai/Documents/GitHub/CSAPP/sim/seq/ssim.c -o CMakeFiles/sim.dir/seq/ssim.c.s

CMakeFiles/sim.dir/seq/ssim.c.o.requires:

.PHONY : CMakeFiles/sim.dir/seq/ssim.c.o.requires

CMakeFiles/sim.dir/seq/ssim.c.o.provides: CMakeFiles/sim.dir/seq/ssim.c.o.requires
	$(MAKE) -f CMakeFiles/sim.dir/build.make CMakeFiles/sim.dir/seq/ssim.c.o.provides.build
.PHONY : CMakeFiles/sim.dir/seq/ssim.c.o.provides

CMakeFiles/sim.dir/seq/ssim.c.o.provides.build: CMakeFiles/sim.dir/seq/ssim.c.o


# Object files for target sim
sim_OBJECTS = \
"CMakeFiles/sim.dir/misc/examples.c.o" \
"CMakeFiles/sim.dir/misc/isa.c.o" \
"CMakeFiles/sim.dir/misc/node.c.o" \
"CMakeFiles/sim.dir/misc/outgen.c.o" \
"CMakeFiles/sim.dir/misc/yas-grammar.c.o" \
"CMakeFiles/sim.dir/misc/yas.c.o" \
"CMakeFiles/sim.dir/misc/yis.c.o" \
"CMakeFiles/sim.dir/pipe/ncopy.c.o" \
"CMakeFiles/sim.dir/pipe/psim.c.o" \
"CMakeFiles/sim.dir/seq/ssim.c.o"

# External object files for target sim
sim_EXTERNAL_OBJECTS = \
"/Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/isa.o" \
"/Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/yas-grammar.o" \
"/Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/yas.o" \
"/Users/chenxuehai/Documents/GitHub/CSAPP/sim/misc/yis.o"

sim: CMakeFiles/sim.dir/misc/examples.c.o
sim: CMakeFiles/sim.dir/misc/isa.c.o
sim: CMakeFiles/sim.dir/misc/node.c.o
sim: CMakeFiles/sim.dir/misc/outgen.c.o
sim: CMakeFiles/sim.dir/misc/yas-grammar.c.o
sim: CMakeFiles/sim.dir/misc/yas.c.o
sim: CMakeFiles/sim.dir/misc/yis.c.o
sim: CMakeFiles/sim.dir/pipe/ncopy.c.o
sim: CMakeFiles/sim.dir/pipe/psim.c.o
sim: CMakeFiles/sim.dir/seq/ssim.c.o
sim: ../misc/isa.o
sim: ../misc/yas-grammar.o
sim: ../misc/yas.o
sim: ../misc/yis.o
sim: CMakeFiles/sim.dir/build.make
sim: CMakeFiles/sim.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking C executable sim"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sim.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sim.dir/build: sim

.PHONY : CMakeFiles/sim.dir/build

CMakeFiles/sim.dir/requires: CMakeFiles/sim.dir/misc/examples.c.o.requires
CMakeFiles/sim.dir/requires: CMakeFiles/sim.dir/misc/isa.c.o.requires
CMakeFiles/sim.dir/requires: CMakeFiles/sim.dir/misc/node.c.o.requires
CMakeFiles/sim.dir/requires: CMakeFiles/sim.dir/misc/outgen.c.o.requires
CMakeFiles/sim.dir/requires: CMakeFiles/sim.dir/misc/yas-grammar.c.o.requires
CMakeFiles/sim.dir/requires: CMakeFiles/sim.dir/misc/yas.c.o.requires
CMakeFiles/sim.dir/requires: CMakeFiles/sim.dir/misc/yis.c.o.requires
CMakeFiles/sim.dir/requires: CMakeFiles/sim.dir/pipe/ncopy.c.o.requires
CMakeFiles/sim.dir/requires: CMakeFiles/sim.dir/pipe/psim.c.o.requires
CMakeFiles/sim.dir/requires: CMakeFiles/sim.dir/seq/ssim.c.o.requires

.PHONY : CMakeFiles/sim.dir/requires

CMakeFiles/sim.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sim.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sim.dir/clean

CMakeFiles/sim.dir/depend:
	cd /Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/chenxuehai/Documents/GitHub/CSAPP/sim /Users/chenxuehai/Documents/GitHub/CSAPP/sim /Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug /Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug /Users/chenxuehai/Documents/GitHub/CSAPP/sim/cmake-build-debug/CMakeFiles/sim.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sim.dir/depend

