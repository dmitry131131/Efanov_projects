# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_SOURCE_DIR = /home/cybermeatball/Files/Efanov/Efanov_projects/client_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/build

# Include any dependencies generated for this target.
include CMakeFiles/Client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Client.dir/flags.make

CMakeFiles/Client.dir/codegen:
.PHONY : CMakeFiles/Client.dir/codegen

CMakeFiles/Client.dir/client/src/main.c.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/client/src/main.c.o: /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/client/src/main.c
CMakeFiles/Client.dir/client/src/main.c.o: CMakeFiles/Client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cybermeatball/Files/Efanov/Efanov_projects/client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Client.dir/client/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Client.dir/client/src/main.c.o -MF CMakeFiles/Client.dir/client/src/main.c.o.d -o CMakeFiles/Client.dir/client/src/main.c.o -c /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/client/src/main.c

CMakeFiles/Client.dir/client/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Client.dir/client/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/client/src/main.c > CMakeFiles/Client.dir/client/src/main.c.i

CMakeFiles/Client.dir/client/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Client.dir/client/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/client/src/main.c -o CMakeFiles/Client.dir/client/src/main.c.s

CMakeFiles/Client.dir/common/src/buffer.c.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/common/src/buffer.c.o: /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/common/src/buffer.c
CMakeFiles/Client.dir/common/src/buffer.c.o: CMakeFiles/Client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cybermeatball/Files/Efanov/Efanov_projects/client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Client.dir/common/src/buffer.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Client.dir/common/src/buffer.c.o -MF CMakeFiles/Client.dir/common/src/buffer.c.o.d -o CMakeFiles/Client.dir/common/src/buffer.c.o -c /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/common/src/buffer.c

CMakeFiles/Client.dir/common/src/buffer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Client.dir/common/src/buffer.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/common/src/buffer.c > CMakeFiles/Client.dir/common/src/buffer.c.i

CMakeFiles/Client.dir/common/src/buffer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Client.dir/common/src/buffer.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/common/src/buffer.c -o CMakeFiles/Client.dir/common/src/buffer.c.s

# Object files for target Client
Client_OBJECTS = \
"CMakeFiles/Client.dir/client/src/main.c.o" \
"CMakeFiles/Client.dir/common/src/buffer.c.o"

# External object files for target Client
Client_EXTERNAL_OBJECTS =

bin/Client: CMakeFiles/Client.dir/client/src/main.c.o
bin/Client: CMakeFiles/Client.dir/common/src/buffer.c.o
bin/Client: CMakeFiles/Client.dir/build.make
bin/Client: CMakeFiles/Client.dir/compiler_depend.ts
bin/Client: CMakeFiles/Client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/cybermeatball/Files/Efanov/Efanov_projects/client_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable bin/Client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Client.dir/build: bin/Client
.PHONY : CMakeFiles/Client.dir/build

CMakeFiles/Client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Client.dir/clean

CMakeFiles/Client.dir/depend:
	cd /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cybermeatball/Files/Efanov/Efanov_projects/client_server /home/cybermeatball/Files/Efanov/Efanov_projects/client_server /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/build /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/build /home/cybermeatball/Files/Efanov/Efanov_projects/client_server/build/CMakeFiles/Client.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Client.dir/depend
