# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.7.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.7.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/chihiro/Programs/opencv/opencv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/chihiro/Programs/opencv/opencv/release

# Include any dependencies generated for this target.
include modules/stereo/CMakeFiles/opencv_test_stereo.dir/depend.make

# Include the progress variables for this target.
include modules/stereo/CMakeFiles/opencv_test_stereo.dir/progress.make

# Include the compile flags for this target's objects.
include modules/stereo/CMakeFiles/opencv_test_stereo.dir/flags.make

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o: modules/stereo/CMakeFiles/opencv_test_stereo.dir/flags.make
modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o: /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo/test/test_block_matching.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chihiro/Programs/opencv/opencv/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o"
	cd /Users/chihiro/Programs/opencv/opencv/release/modules/stereo && /Library/Developer/CommandLineTools/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o -c /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo/test/test_block_matching.cpp

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.i"
	cd /Users/chihiro/Programs/opencv/opencv/release/modules/stereo && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo/test/test_block_matching.cpp > CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.i

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.s"
	cd /Users/chihiro/Programs/opencv/opencv/release/modules/stereo && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo/test/test_block_matching.cpp -o CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.s

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o.requires:

.PHONY : modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o.requires

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o.provides: modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o.requires
	$(MAKE) -f modules/stereo/CMakeFiles/opencv_test_stereo.dir/build.make modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o.provides.build
.PHONY : modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o.provides

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o.provides.build: modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o


modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o: modules/stereo/CMakeFiles/opencv_test_stereo.dir/flags.make
modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o: /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo/test/test_descriptors.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chihiro/Programs/opencv/opencv/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o"
	cd /Users/chihiro/Programs/opencv/opencv/release/modules/stereo && /Library/Developer/CommandLineTools/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o -c /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo/test/test_descriptors.cpp

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.i"
	cd /Users/chihiro/Programs/opencv/opencv/release/modules/stereo && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo/test/test_descriptors.cpp > CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.i

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.s"
	cd /Users/chihiro/Programs/opencv/opencv/release/modules/stereo && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo/test/test_descriptors.cpp -o CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.s

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o.requires:

.PHONY : modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o.requires

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o.provides: modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o.requires
	$(MAKE) -f modules/stereo/CMakeFiles/opencv_test_stereo.dir/build.make modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o.provides.build
.PHONY : modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o.provides

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o.provides.build: modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o


modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o: modules/stereo/CMakeFiles/opencv_test_stereo.dir/flags.make
modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o: /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo/test/test_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chihiro/Programs/opencv/opencv/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o"
	cd /Users/chihiro/Programs/opencv/opencv/release/modules/stereo && /Library/Developer/CommandLineTools/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o -c /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo/test/test_main.cpp

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.i"
	cd /Users/chihiro/Programs/opencv/opencv/release/modules/stereo && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo/test/test_main.cpp > CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.i

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.s"
	cd /Users/chihiro/Programs/opencv/opencv/release/modules/stereo && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo/test/test_main.cpp -o CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.s

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o.requires:

.PHONY : modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o.requires

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o.provides: modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o.requires
	$(MAKE) -f modules/stereo/CMakeFiles/opencv_test_stereo.dir/build.make modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o.provides.build
.PHONY : modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o.provides

modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o.provides.build: modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o


# Object files for target opencv_test_stereo
opencv_test_stereo_OBJECTS = \
"CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o" \
"CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o" \
"CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o"

# External object files for target opencv_test_stereo
opencv_test_stereo_EXTERNAL_OBJECTS =

bin/opencv_test_stereo: modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o
bin/opencv_test_stereo: modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o
bin/opencv_test_stereo: modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o
bin/opencv_test_stereo: modules/stereo/CMakeFiles/opencv_test_stereo.dir/build.make
bin/opencv_test_stereo: lib/libopencv_ts.a
bin/opencv_test_stereo: lib/libopencv_stereo.3.2.0.dylib
bin/opencv_test_stereo: lib/libopencv_calib3d.3.2.0.dylib
bin/opencv_test_stereo: 3rdparty/ippicv/ippicv_osx/lib/libippicv.a
bin/opencv_test_stereo: lib/libopencv_features2d.3.2.0.dylib
bin/opencv_test_stereo: lib/libopencv_flann.3.2.0.dylib
bin/opencv_test_stereo: lib/libopencv_ml.3.2.0.dylib
bin/opencv_test_stereo: lib/libopencv_highgui.3.2.0.dylib
bin/opencv_test_stereo: lib/libopencv_videoio.3.2.0.dylib
bin/opencv_test_stereo: lib/libopencv_imgcodecs.3.2.0.dylib
bin/opencv_test_stereo: lib/libopencv_imgproc.3.2.0.dylib
bin/opencv_test_stereo: lib/libopencv_core.3.2.0.dylib
bin/opencv_test_stereo: modules/stereo/CMakeFiles/opencv_test_stereo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/chihiro/Programs/opencv/opencv/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../../bin/opencv_test_stereo"
	cd /Users/chihiro/Programs/opencv/opencv/release/modules/stereo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/opencv_test_stereo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
modules/stereo/CMakeFiles/opencv_test_stereo.dir/build: bin/opencv_test_stereo

.PHONY : modules/stereo/CMakeFiles/opencv_test_stereo.dir/build

modules/stereo/CMakeFiles/opencv_test_stereo.dir/requires: modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_block_matching.cpp.o.requires
modules/stereo/CMakeFiles/opencv_test_stereo.dir/requires: modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_descriptors.cpp.o.requires
modules/stereo/CMakeFiles/opencv_test_stereo.dir/requires: modules/stereo/CMakeFiles/opencv_test_stereo.dir/test/test_main.cpp.o.requires

.PHONY : modules/stereo/CMakeFiles/opencv_test_stereo.dir/requires

modules/stereo/CMakeFiles/opencv_test_stereo.dir/clean:
	cd /Users/chihiro/Programs/opencv/opencv/release/modules/stereo && $(CMAKE_COMMAND) -P CMakeFiles/opencv_test_stereo.dir/cmake_clean.cmake
.PHONY : modules/stereo/CMakeFiles/opencv_test_stereo.dir/clean

modules/stereo/CMakeFiles/opencv_test_stereo.dir/depend:
	cd /Users/chihiro/Programs/opencv/opencv/release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/chihiro/Programs/opencv/opencv /Users/chihiro/Programs/opencv/opencv_contrib/modules/stereo /Users/chihiro/Programs/opencv/opencv/release /Users/chihiro/Programs/opencv/opencv/release/modules/stereo /Users/chihiro/Programs/opencv/opencv/release/modules/stereo/CMakeFiles/opencv_test_stereo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : modules/stereo/CMakeFiles/opencv_test_stereo.dir/depend

