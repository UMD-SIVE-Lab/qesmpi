cmake_minimum_required (VERSION 2.6) 
project (optimizationCode)

#Setting compiler to use C++11
SET (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
SET (CMAKE_BUILD_TYPE Debug)

#set libsivelab path
SET(LIBSIVELAB_PATH "${CMAKE_SOURCE_DIR}/libsivelab" CACHE STRING "Path to the directory where the libsivelab repository is located (assumes code is built into a directory named 'build').")

#MPI 
FIND_PACKAGE(MPI REQUIRED)

#BOOST
SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost COMPONENTS mpi serialization REQUIRED)

#add sub directory
add_subdirectory(${CMAKE_SOURCE_DIR}/opt_grammar)

#include_directories
include_directories (${Boost_INCLUDE_DIRS})
include_directories (${MPI_CXX_INCLUDE_PATH})
include_directories (${LIBSIVELAB_PATH}) 
include_directories (${CMAKE_SOURCE_DIR}/opt_grammar)
include_directories (${CMAKE_SOURCE_DIR}/opt_grammar/antlr)

#
#add code such that cmake automatically compiles lexer and parser
#

#link include_directories
LINK_DIRECTORIES (${LIBSIVELAB_PATH}/lib)

#build executable 
# add_executable(quic quic.cpp
# 					ArgumentParsing.cpp
# 					master.cpp
# 					FitnessCache.cpp
# 					population_gen.cpp
# 					samples.cpp
# 					directory.cpp
# 				)

add_executable(quic quic.cpp
					ArgumentParsing.cpp
					gpu_plume_job.cpp
					population_gen.cpp
					population.cpp
					directory.cpp
					FitnessCache.cpp
					concentrationRedux.cpp
			)

#libraries 
target_link_libraries( quic ${MPI_LIBRARIES})
target_link_libraries( quic ${Boost_LIBRARIES})
target_link_libraries (quic sive-quicutil)
target_link_libraries (quic sive-util) 
target_link_libraries (quic optfileparser)
target_link_libraries (quic dl)

