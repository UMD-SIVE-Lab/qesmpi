cmake_minimum_required (VERSION 2.8) 
project (optimizationCode)

#MPI 
FIND_PACKAGE(MPI REQUIRED)

#BOOST
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREADED OFF)
FIND_PACKAGE(Boost COMPONENTS mpi serialization timer chrono REQUIRED)

#add sub directory
message(STATUS "Adding opt_grammar directory...")
add_subdirectory(opt_grammar)

message(STATUS "Adding utils directory...")
add_subdirectory(utils)

#fitness function shared library location
add_definitions( -DFITNESS_FUNCTION_LIBRARY="${CMAKE_BINARY_DIR}/fitness_lib/libfitness.so" )


message(STATUS "Adding MPI framework directory...")
add_subdirectory(MPI_framework)

#message(STATUS "Adding MPI gpuplume directory...")
#add_subdirectory(mpi_gpuplume)


message(STATUS "Adding MPI LSM directory...")
add_subdirectory(mpi_qes_spf)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#include_directories
include_directories (${Boost_INCLUDE_DIRS})
include_directories (${MPI_CXX_INCLUDE_PATH})
include_directories (${LIBSIVELAB_PATH}) 
include_directories (${CMAKE_SOURCE_DIR}/include)
include_directories (${CMAKE_SOURCE_DIR}/fitness_lib/include) 
include_directories (opt_grammar)
include_directories (opt_grammar/antlr)
include_directories (${QES_INCLUDE_DIRECTORIES})

#link include_directories
LINK_DIRECTORIES (${LIBSIVELAB_PATH}/lib)

message(STATUS "Linking the executable with necessary libraries")
add_executable(quic quic.cpp)

#libraries 
target_link_libraries (quic ${MPI_LIBRARIES})
target_link_libraries (quic ${Boost_LIBRARIES})
target_link_libraries (quic optfileparser)
#target_link_libraries (quic mpi_gpuplume)
target_link_libraries (quic mpi_simpleLSM)
target_link_libraries (quic mpi_framework)
target_link_libraries (quic optm-utils)
target_link_libraries (quic sive-quicutil)
target_link_libraries (quic sive-util) 