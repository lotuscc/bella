# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /home/lotuscc/github_Project/bella

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lotuscc/github_Project/bella/build

# Include any dependencies generated for this target.
include CMakeFiles/server.out.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server.out.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.out.dir/flags.make

CMakeFiles/server.out.dir/server/main.cpp.o: CMakeFiles/server.out.dir/flags.make
CMakeFiles/server.out.dir/server/main.cpp.o: ../server/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lotuscc/github_Project/bella/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server.out.dir/server/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.out.dir/server/main.cpp.o -c /home/lotuscc/github_Project/bella/server/main.cpp

CMakeFiles/server.out.dir/server/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.out.dir/server/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lotuscc/github_Project/bella/server/main.cpp > CMakeFiles/server.out.dir/server/main.cpp.i

CMakeFiles/server.out.dir/server/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.out.dir/server/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lotuscc/github_Project/bella/server/main.cpp -o CMakeFiles/server.out.dir/server/main.cpp.s

CMakeFiles/server.out.dir/protobuf/ell_message.pb.cc.o: CMakeFiles/server.out.dir/flags.make
CMakeFiles/server.out.dir/protobuf/ell_message.pb.cc.o: ../protobuf/ell_message.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lotuscc/github_Project/bella/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/server.out.dir/protobuf/ell_message.pb.cc.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.out.dir/protobuf/ell_message.pb.cc.o -c /home/lotuscc/github_Project/bella/protobuf/ell_message.pb.cc

CMakeFiles/server.out.dir/protobuf/ell_message.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.out.dir/protobuf/ell_message.pb.cc.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lotuscc/github_Project/bella/protobuf/ell_message.pb.cc > CMakeFiles/server.out.dir/protobuf/ell_message.pb.cc.i

CMakeFiles/server.out.dir/protobuf/ell_message.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.out.dir/protobuf/ell_message.pb.cc.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lotuscc/github_Project/bella/protobuf/ell_message.pb.cc -o CMakeFiles/server.out.dir/protobuf/ell_message.pb.cc.s

# Object files for target server.out
server_out_OBJECTS = \
"CMakeFiles/server.out.dir/server/main.cpp.o" \
"CMakeFiles/server.out.dir/protobuf/ell_message.pb.cc.o"

# External object files for target server.out
server_out_EXTERNAL_OBJECTS =

server.out: CMakeFiles/server.out.dir/server/main.cpp.o
server.out: CMakeFiles/server.out.dir/protobuf/ell_message.pb.cc.o
server.out: CMakeFiles/server.out.dir/build.make
server.out: /usr/local/lib/libprotobuf.so
server.out: /usr/local/lib/libprotobuf.so
server.out: CMakeFiles/server.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lotuscc/github_Project/bella/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable server.out"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.out.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.out.dir/build: server.out

.PHONY : CMakeFiles/server.out.dir/build

CMakeFiles/server.out.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.out.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.out.dir/clean

CMakeFiles/server.out.dir/depend:
	cd /home/lotuscc/github_Project/bella/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lotuscc/github_Project/bella /home/lotuscc/github_Project/bella /home/lotuscc/github_Project/bella/build /home/lotuscc/github_Project/bella/build /home/lotuscc/github_Project/bella/build/CMakeFiles/server.out.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server.out.dir/depend

