# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection

# Include any dependencies generated for this target.
include CMakeFiles/grooveDetection.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/grooveDetection.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/grooveDetection.dir/flags.make

CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o: CMakeFiles/grooveDetection.dir/flags.make
CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o: grooveDetection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o -c /home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection/grooveDetection.cpp

CMakeFiles/grooveDetection.dir/grooveDetection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grooveDetection.dir/grooveDetection.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection/grooveDetection.cpp > CMakeFiles/grooveDetection.dir/grooveDetection.cpp.i

CMakeFiles/grooveDetection.dir/grooveDetection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grooveDetection.dir/grooveDetection.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection/grooveDetection.cpp -o CMakeFiles/grooveDetection.dir/grooveDetection.cpp.s

CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o.requires:

.PHONY : CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o.requires

CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o.provides: CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o.requires
	$(MAKE) -f CMakeFiles/grooveDetection.dir/build.make CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o.provides.build
.PHONY : CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o.provides

CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o.provides.build: CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o


# Object files for target grooveDetection
grooveDetection_OBJECTS = \
"CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o"

# External object files for target grooveDetection
grooveDetection_EXTERNAL_OBJECTS =

grooveDetection: CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o
grooveDetection: CMakeFiles/grooveDetection.dir/build.make
grooveDetection: /usr/local/lib/libopencv_photo.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_objdetect.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_video.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_highgui.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_ml.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_stitching.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_dnn.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_videoio.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_gapi.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_imgcodecs.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_calib3d.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_features2d.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_flann.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_imgproc.so.4.2.0
grooveDetection: /usr/local/lib/libopencv_core.so.4.2.0
grooveDetection: CMakeFiles/grooveDetection.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable grooveDetection"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/grooveDetection.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/grooveDetection.dir/build: grooveDetection

.PHONY : CMakeFiles/grooveDetection.dir/build

CMakeFiles/grooveDetection.dir/requires: CMakeFiles/grooveDetection.dir/grooveDetection.cpp.o.requires

.PHONY : CMakeFiles/grooveDetection.dir/requires

CMakeFiles/grooveDetection.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/grooveDetection.dir/cmake_clean.cmake
.PHONY : CMakeFiles/grooveDetection.dir/clean

CMakeFiles/grooveDetection.dir/depend:
	cd /home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection /home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection /home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection /home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection /home/nick/Documents/SeniorDesign/ComputerVisionTest/GrooveDetection/CMakeFiles/grooveDetection.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/grooveDetection.dir/depend

