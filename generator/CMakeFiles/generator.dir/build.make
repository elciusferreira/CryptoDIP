# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator

# Include any dependencies generated for this target.
include CMakeFiles/generator.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/generator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/generator.dir/flags.make

CMakeFiles/generator.dir/generator.cpp.o: CMakeFiles/generator.dir/flags.make
CMakeFiles/generator.dir/generator.cpp.o: generator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/generator.dir/generator.cpp.o"
	g++-5  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/generator.dir/generator.cpp.o -c /home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator/generator.cpp

CMakeFiles/generator.dir/generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/generator.dir/generator.cpp.i"
	g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator/generator.cpp > CMakeFiles/generator.dir/generator.cpp.i

CMakeFiles/generator.dir/generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/generator.dir/generator.cpp.s"
	g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator/generator.cpp -o CMakeFiles/generator.dir/generator.cpp.s

# Object files for target generator
generator_OBJECTS = \
"CMakeFiles/generator.dir/generator.cpp.o"

# External object files for target generator
generator_EXTERNAL_OBJECTS =

generator: CMakeFiles/generator.dir/generator.cpp.o
generator: CMakeFiles/generator.dir/build.make
generator: /usr/local/lib/libopencv_stitching.so.3.4.1
generator: /usr/local/lib/libopencv_superres.so.3.4.1
generator: /usr/local/lib/libopencv_videostab.so.3.4.1
generator: /usr/local/lib/libopencv_dnn_objdetect.so.3.4.1
generator: /usr/local/lib/libopencv_xphoto.so.3.4.1
generator: /usr/local/lib/libopencv_hfs.so.3.4.1
generator: /usr/local/lib/libopencv_ccalib.so.3.4.1
generator: /usr/local/lib/libopencv_img_hash.so.3.4.1
generator: /usr/local/lib/libopencv_hdf.so.3.4.1
generator: /usr/local/lib/libopencv_structured_light.so.3.4.1
generator: /usr/local/lib/libopencv_aruco.so.3.4.1
generator: /usr/local/lib/libopencv_reg.so.3.4.1
generator: /usr/local/lib/libopencv_line_descriptor.so.3.4.1
generator: /usr/local/lib/libopencv_dpm.so.3.4.1
generator: /usr/local/lib/libopencv_fuzzy.so.3.4.1
generator: /usr/local/lib/libopencv_stereo.so.3.4.1
generator: /usr/local/lib/libopencv_bgsegm.so.3.4.1
generator: /usr/local/lib/libopencv_rgbd.so.3.4.1
generator: /usr/local/lib/libopencv_face.so.3.4.1
generator: /usr/local/lib/libopencv_saliency.so.3.4.1
generator: /usr/local/lib/libopencv_xobjdetect.so.3.4.1
generator: /usr/local/lib/libopencv_xfeatures2d.so.3.4.1
generator: /usr/local/lib/libopencv_phase_unwrapping.so.3.4.1
generator: /usr/local/lib/libopencv_surface_matching.so.3.4.1
generator: /usr/local/lib/libopencv_bioinspired.so.3.4.1
generator: /usr/local/lib/libopencv_optflow.so.3.4.1
generator: /usr/local/lib/libopencv_freetype.so.3.4.1
generator: /usr/local/lib/libopencv_shape.so.3.4.1
generator: /usr/local/lib/libopencv_photo.so.3.4.1
generator: /usr/local/lib/libopencv_tracking.so.3.4.1
generator: /usr/local/lib/libopencv_datasets.so.3.4.1
generator: /usr/local/lib/libopencv_plot.so.3.4.1
generator: /usr/local/lib/libopencv_text.so.3.4.1
generator: /usr/local/lib/libopencv_ml.so.3.4.1
generator: /usr/local/lib/libopencv_dnn.so.3.4.1
generator: /usr/local/lib/libopencv_objdetect.so.3.4.1
generator: /usr/local/lib/libopencv_video.so.3.4.1
generator: /usr/local/lib/libopencv_ximgproc.so.3.4.1
generator: /usr/local/lib/libopencv_calib3d.so.3.4.1
generator: /usr/local/lib/libopencv_features2d.so.3.4.1
generator: /usr/local/lib/libopencv_highgui.so.3.4.1
generator: /usr/local/lib/libopencv_flann.so.3.4.1
generator: /usr/local/lib/libopencv_videoio.so.3.4.1
generator: /usr/local/lib/libopencv_imgcodecs.so.3.4.1
generator: /usr/local/lib/libopencv_imgproc.so.3.4.1
generator: /usr/local/lib/libopencv_core.so.3.4.1
generator: CMakeFiles/generator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable generator"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/generator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/generator.dir/build: generator

.PHONY : CMakeFiles/generator.dir/build

CMakeFiles/generator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/generator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/generator.dir/clean

CMakeFiles/generator.dir/depend:
	cd /home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator /home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator /home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator /home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator /home/johannes/Documentos/P8/Coprojeto_HW_SW/Projetos/CryptoDIP/generator/CMakeFiles/generator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/generator.dir/depend

