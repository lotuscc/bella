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
include CMakeFiles/client.out.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client.out.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.out.dir/flags.make

CMakeFiles/client.out.dir/client/main.cpp.o: CMakeFiles/client.out.dir/flags.make
CMakeFiles/client.out.dir/client/main.cpp.o: ../client/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lotuscc/github_Project/bella/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client.out.dir/client/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.out.dir/client/main.cpp.o -c /home/lotuscc/github_Project/bella/client/main.cpp

CMakeFiles/client.out.dir/client/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.out.dir/client/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lotuscc/github_Project/bella/client/main.cpp > CMakeFiles/client.out.dir/client/main.cpp.i

CMakeFiles/client.out.dir/client/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.out.dir/client/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lotuscc/github_Project/bella/client/main.cpp -o CMakeFiles/client.out.dir/client/main.cpp.s

CMakeFiles/client.out.dir/src/ell_Channel.cc.o: CMakeFiles/client.out.dir/flags.make
CMakeFiles/client.out.dir/src/ell_Channel.cc.o: ../src/ell_Channel.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lotuscc/github_Project/bella/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/client.out.dir/src/ell_Channel.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.out.dir/src/ell_Channel.cc.o -c /home/lotuscc/github_Project/bella/src/ell_Channel.cc

CMakeFiles/client.out.dir/src/ell_Channel.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.out.dir/src/ell_Channel.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lotuscc/github_Project/bella/src/ell_Channel.cc > CMakeFiles/client.out.dir/src/ell_Channel.cc.i

CMakeFiles/client.out.dir/src/ell_Channel.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.out.dir/src/ell_Channel.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lotuscc/github_Project/bella/src/ell_Channel.cc -o CMakeFiles/client.out.dir/src/ell_Channel.cc.s

CMakeFiles/client.out.dir/src/ell_EPoller.cc.o: CMakeFiles/client.out.dir/flags.make
CMakeFiles/client.out.dir/src/ell_EPoller.cc.o: ../src/ell_EPoller.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lotuscc/github_Project/bella/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/client.out.dir/src/ell_EPoller.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.out.dir/src/ell_EPoller.cc.o -c /home/lotuscc/github_Project/bella/src/ell_EPoller.cc

CMakeFiles/client.out.dir/src/ell_EPoller.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.out.dir/src/ell_EPoller.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lotuscc/github_Project/bella/src/ell_EPoller.cc > CMakeFiles/client.out.dir/src/ell_EPoller.cc.i

CMakeFiles/client.out.dir/src/ell_EPoller.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.out.dir/src/ell_EPoller.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lotuscc/github_Project/bella/src/ell_EPoller.cc -o CMakeFiles/client.out.dir/src/ell_EPoller.cc.s

CMakeFiles/client.out.dir/protobuf/ell_message.pb.cc.o: CMakeFiles/client.out.dir/flags.make
CMakeFiles/client.out.dir/protobuf/ell_message.pb.cc.o: ../protobuf/ell_message.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lotuscc/github_Project/bella/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/client.out.dir/protobuf/ell_message.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.out.dir/protobuf/ell_message.pb.cc.o -c /home/lotuscc/github_Project/bella/protobuf/ell_message.pb.cc

CMakeFiles/client.out.dir/protobuf/ell_message.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.out.dir/protobuf/ell_message.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lotuscc/github_Project/bella/protobuf/ell_message.pb.cc > CMakeFiles/client.out.dir/protobuf/ell_message.pb.cc.i

CMakeFiles/client.out.dir/protobuf/ell_message.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.out.dir/protobuf/ell_message.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lotuscc/github_Project/bella/protobuf/ell_message.pb.cc -o CMakeFiles/client.out.dir/protobuf/ell_message.pb.cc.s

# Object files for target client.out
client_out_OBJECTS = \
"CMakeFiles/client.out.dir/client/main.cpp.o" \
"CMakeFiles/client.out.dir/src/ell_Channel.cc.o" \
"CMakeFiles/client.out.dir/src/ell_EPoller.cc.o" \
"CMakeFiles/client.out.dir/protobuf/ell_message.pb.cc.o"

# External object files for target client.out
client_out_EXTERNAL_OBJECTS =

client.out: CMakeFiles/client.out.dir/client/main.cpp.o
client.out: CMakeFiles/client.out.dir/src/ell_Channel.cc.o
client.out: CMakeFiles/client.out.dir/src/ell_EPoller.cc.o
client.out: CMakeFiles/client.out.dir/protobuf/ell_message.pb.cc.o
client.out: CMakeFiles/client.out.dir/build.make
client.out: /usr/local/lib/libprotobuf.so
client.out: /usr/local/lib/libprotobuf.so
client.out: CMakeFiles/client.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lotuscc/github_Project/bella/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable client.out"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.out.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.out.dir/build: client.out

.PHONY : CMakeFiles/client.out.dir/build

CMakeFiles/client.out.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.out.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.out.dir/clean

CMakeFiles/client.out.dir/depend:
	cd /home/lotuscc/github_Project/bella/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lotuscc/github_Project/bella /home/lotuscc/github_Project/bella /home/lotuscc/github_Project/bella/build /home/lotuscc/github_Project/bella/build /home/lotuscc/github_Project/bella/build/CMakeFiles/client.out.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client.out.dir/depend

