cmake_minimum_required (VERSION 2.8) 
project (optimizationCode)

#Setting compiler to use C++11
SET (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
SET (CMAKE_BUILD_TYPE Debug)

#building dependencies

#include matts libraries
SET(LIBSIVELAB_PATH "${CMAKE_SOURCE_DIR}/../libsivelab" CACHE STRING "Path to the directory where the libsivelab repository is located (assumes code is built into a directory named 'build').")
SET(QUIC_DATA_PATH "${CMAKE_SOURCE_DIR}/../quicdata" CACHE STRING "Directory path of cmake")

#fintess function shared library
add_subdirectory(fitness_lib)

include(ExternalProject)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ExternalProject_Add(
# 	qes_spf
# 	PREFIX ${CMAKE_BINARY_DIR}/qes-v1
# 	#--Download Step--
# 	SVN_REPOSITORY https://wind.d.umn.edu/svn/quic_envsim/branches/qes-v1
# 	SVN_USERNAME vuggu001
# 	SVN_TRUST_CERT 1
# 	#--Update step--
# 	UPDATE COMMAND "svn update" 
# 	#--Configure Step
# 	CMAKE_ARGS -DLIBRARY_OUTPUT_PATH=${CMAKE_SOURCE_DIR}/lib/qes_lib 
# 			   -DCUDA_TOOLKIT_ROOT_DIR=/home/cs/software/sivelab/cuda_6.0
# 			   -DOptiX_INSTALL_DIR=/home/cs/software/sivelab/NVIDIA-OptiX-SDK-3.6.2-linux64
# 			   -DQUIC_DATA_PATH=${QUIC_DATA_PATH}

# 	#--Build step--
# 	BINARY_DIR "build"
# 	BUILD_COMMAND make
# 	#--install step--
# 	#we do not need any installation
# 	INSTALL_COMMAND ""
# )

# ExternalProject_Get_Property(qes_spf source_dir)
# include_directories(${source_dir}/include)

add_subdirectory(src)