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
CMAKE_SOURCE_DIR = /home/dushn/Diplomski/diplomski/dserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dushn/Diplomski/diplomski/dserver/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.dir/flags.make

CMakeFiles/server.dir/main.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/server.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/main.c.o   -c /home/dushn/Diplomski/diplomski/dserver/main.c

CMakeFiles/server.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dserver/main.c > CMakeFiles/server.dir/main.c.i

CMakeFiles/server.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dserver/main.c -o CMakeFiles/server.dir/main.c.s

CMakeFiles/server.dir/cpu_usage.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/cpu_usage.c.o: ../cpu_usage.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/server.dir/cpu_usage.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/cpu_usage.c.o   -c /home/dushn/Diplomski/diplomski/dserver/cpu_usage.c

CMakeFiles/server.dir/cpu_usage.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/cpu_usage.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dserver/cpu_usage.c > CMakeFiles/server.dir/cpu_usage.c.i

CMakeFiles/server.dir/cpu_usage.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/cpu_usage.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dserver/cpu_usage.c -o CMakeFiles/server.dir/cpu_usage.c.s

CMakeFiles/server.dir/devices.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/devices.c.o: ../devices.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/server.dir/devices.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/devices.c.o   -c /home/dushn/Diplomski/diplomski/dserver/devices.c

CMakeFiles/server.dir/devices.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/devices.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dserver/devices.c > CMakeFiles/server.dir/devices.c.i

CMakeFiles/server.dir/devices.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/devices.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dserver/devices.c -o CMakeFiles/server.dir/devices.c.s

CMakeFiles/server.dir/functions.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/functions.c.o: ../functions.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/server.dir/functions.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/functions.c.o   -c /home/dushn/Diplomski/diplomski/dserver/functions.c

CMakeFiles/server.dir/functions.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/functions.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dserver/functions.c > CMakeFiles/server.dir/functions.c.i

CMakeFiles/server.dir/functions.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/functions.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dserver/functions.c -o CMakeFiles/server.dir/functions.c.s

CMakeFiles/server.dir/interrupts.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/interrupts.c.o: ../interrupts.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/server.dir/interrupts.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/interrupts.c.o   -c /home/dushn/Diplomski/diplomski/dserver/interrupts.c

CMakeFiles/server.dir/interrupts.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/interrupts.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dserver/interrupts.c > CMakeFiles/server.dir/interrupts.c.i

CMakeFiles/server.dir/interrupts.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/interrupts.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dserver/interrupts.c -o CMakeFiles/server.dir/interrupts.c.s

CMakeFiles/server.dir/memory_usage.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/memory_usage.c.o: ../memory_usage.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/server.dir/memory_usage.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/memory_usage.c.o   -c /home/dushn/Diplomski/diplomski/dserver/memory_usage.c

CMakeFiles/server.dir/memory_usage.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/memory_usage.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dserver/memory_usage.c > CMakeFiles/server.dir/memory_usage.c.i

CMakeFiles/server.dir/memory_usage.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/memory_usage.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dserver/memory_usage.c -o CMakeFiles/server.dir/memory_usage.c.s

CMakeFiles/server.dir/network_bandwith.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/network_bandwith.c.o: ../network_bandwith.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/server.dir/network_bandwith.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/network_bandwith.c.o   -c /home/dushn/Diplomski/diplomski/dserver/network_bandwith.c

CMakeFiles/server.dir/network_bandwith.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/network_bandwith.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dserver/network_bandwith.c > CMakeFiles/server.dir/network_bandwith.c.i

CMakeFiles/server.dir/network_bandwith.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/network_bandwith.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dserver/network_bandwith.c -o CMakeFiles/server.dir/network_bandwith.c.s

CMakeFiles/server.dir/task_manager.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/task_manager.c.o: ../task_manager.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dushn/Diplomski/diplomski/dserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/server.dir/task_manager.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/task_manager.c.o   -c /home/dushn/Diplomski/diplomski/dserver/task_manager.c

CMakeFiles/server.dir/task_manager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/task_manager.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dushn/Diplomski/diplomski/dserver/task_manager.c > CMakeFiles/server.dir/task_manager.c.i

CMakeFiles/server.dir/task_manager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/task_manager.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dushn/Diplomski/diplomski/dserver/task_manager.c -o CMakeFiles/server.dir/task_manager.c.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/main.c.o" \
"CMakeFiles/server.dir/cpu_usage.c.o" \
"CMakeFiles/server.dir/devices.c.o" \
"CMakeFiles/server.dir/functions.c.o" \
"CMakeFiles/server.dir/interrupts.c.o" \
"CMakeFiles/server.dir/memory_usage.c.o" \
"CMakeFiles/server.dir/network_bandwith.c.o" \
"CMakeFiles/server.dir/task_manager.c.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

server: CMakeFiles/server.dir/main.c.o
server: CMakeFiles/server.dir/cpu_usage.c.o
server: CMakeFiles/server.dir/devices.c.o
server: CMakeFiles/server.dir/functions.c.o
server: CMakeFiles/server.dir/interrupts.c.o
server: CMakeFiles/server.dir/memory_usage.c.o
server: CMakeFiles/server.dir/network_bandwith.c.o
server: CMakeFiles/server.dir/task_manager.c.o
server: CMakeFiles/server.dir/build.make
server: CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dushn/Diplomski/diplomski/dserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking C executable server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.dir/build: server

.PHONY : CMakeFiles/server.dir/build

CMakeFiles/server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.dir/clean

CMakeFiles/server.dir/depend:
	cd /home/dushn/Diplomski/diplomski/dserver/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dushn/Diplomski/diplomski/dserver /home/dushn/Diplomski/diplomski/dserver /home/dushn/Diplomski/diplomski/dserver/cmake-build-debug /home/dushn/Diplomski/diplomski/dserver/cmake-build-debug /home/dushn/Diplomski/diplomski/dserver/cmake-build-debug/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server.dir/depend

