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
CMAKE_COMMAND = /snap/clion/121/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/121/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dushn/Diplomski/diplomski/dclient

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dushn/Diplomski/diplomski/dclient/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/main.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dclient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/client.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/main.c.o   -c /home/dushn/Diplomski/diplomski/dclient/main.c

CMakeFiles/client.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dclient/main.c > CMakeFiles/client.dir/main.c.i

CMakeFiles/client.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dclient/main.c -o CMakeFiles/client.dir/main.c.s

CMakeFiles/client.dir/drawing.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/drawing.c.o: ../drawing.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dclient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/client.dir/drawing.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/drawing.c.o   -c /home/dushn/Diplomski/diplomski/dclient/drawing.c

CMakeFiles/client.dir/drawing.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/drawing.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dclient/drawing.c > CMakeFiles/client.dir/drawing.c.i

CMakeFiles/client.dir/drawing.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/drawing.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dclient/drawing.c -o CMakeFiles/client.dir/drawing.c.s

CMakeFiles/client.dir/buttons.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/buttons.c.o: ../buttons.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dclient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/client.dir/buttons.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/buttons.c.o   -c /home/dushn/Diplomski/diplomski/dclient/buttons.c

CMakeFiles/client.dir/buttons.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/buttons.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dclient/buttons.c > CMakeFiles/client.dir/buttons.c.i

CMakeFiles/client.dir/buttons.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/buttons.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dclient/buttons.c -o CMakeFiles/client.dir/buttons.c.s

CMakeFiles/client.dir/functions.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/functions.c.o: ../functions.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dclient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/client.dir/functions.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/functions.c.o   -c /home/dushn/Diplomski/diplomski/dclient/functions.c

CMakeFiles/client.dir/functions.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/functions.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dclient/functions.c > CMakeFiles/client.dir/functions.c.i

CMakeFiles/client.dir/functions.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/functions.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dclient/functions.c -o CMakeFiles/client.dir/functions.c.s

CMakeFiles/client.dir/window.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/window.c.o: ../window.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dclient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/client.dir/window.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/window.c.o   -c /home/dushn/Diplomski/diplomski/dclient/window.c

CMakeFiles/client.dir/window.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/window.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dclient/window.c > CMakeFiles/client.dir/window.c.i

CMakeFiles/client.dir/window.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/window.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dclient/window.c -o CMakeFiles/client.dir/window.c.s

CMakeFiles/client.dir/testing_tree.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/testing_tree.c.o: ../testing_tree.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dclient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/client.dir/testing_tree.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/testing_tree.c.o   -c /home/dushn/Diplomski/diplomski/dclient/testing_tree.c

CMakeFiles/client.dir/testing_tree.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/testing_tree.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dclient/testing_tree.c > CMakeFiles/client.dir/testing_tree.c.i

CMakeFiles/client.dir/testing_tree.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/testing_tree.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dclient/testing_tree.c -o CMakeFiles/client.dir/testing_tree.c.s

CMakeFiles/client.dir/interrupts.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/interrupts.c.o: ../interrupts.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dclient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/client.dir/interrupts.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/interrupts.c.o   -c /home/dushn/Diplomski/diplomski/dclient/interrupts.c

CMakeFiles/client.dir/interrupts.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/interrupts.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dclient/interrupts.c > CMakeFiles/client.dir/interrupts.c.i

CMakeFiles/client.dir/interrupts.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/interrupts.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dclient/interrupts.c -o CMakeFiles/client.dir/interrupts.c.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/main.c.o" \
"CMakeFiles/client.dir/drawing.c.o" \
"CMakeFiles/client.dir/buttons.c.o" \
"CMakeFiles/client.dir/functions.c.o" \
"CMakeFiles/client.dir/window.c.o" \
"CMakeFiles/client.dir/testing_tree.c.o" \
"CMakeFiles/client.dir/interrupts.c.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client: CMakeFiles/client.dir/main.c.o
client: CMakeFiles/client.dir/drawing.c.o
client: CMakeFiles/client.dir/buttons.c.o
client: CMakeFiles/client.dir/functions.c.o
client: CMakeFiles/client.dir/window.c.o
client: CMakeFiles/client.dir/testing_tree.c.o
client: CMakeFiles/client.dir/interrupts.c.o
client: CMakeFiles/client.dir/build.make
client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dushn/Diplomski/diplomski/dclient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C executable client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client

.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd /home/dushn/Diplomski/diplomski/dclient/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dushn/Diplomski/diplomski/dclient /home/dushn/Diplomski/diplomski/dclient /home/dushn/Diplomski/diplomski/dclient/cmake-build-debug /home/dushn/Diplomski/diplomski/dclient/cmake-build-debug /home/dushn/Diplomski/diplomski/dclient/cmake-build-debug/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend

