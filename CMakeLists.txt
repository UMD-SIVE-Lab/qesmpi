cmake_minimum_required (VERSION 2.6) 
project (optimizationCode)

#Setting compiler to use C++11
SET (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")

#MPI 
FIND_PACKAGE(MPI REQUIRED)

#BOOST
SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost COMPONENTS mpi serialization REQUIRED)

#include_directories
include_directories (${Boost_INCLUDE_DIRS})
include_directories (${MPI_CXX_INCLUDE_PATH})

#build executable 
add_executable(quic quic.cpp
					ArgumentParsing.cpp
					master.cpp
					FitnessCache.cpp
					population_gen.cpp
					samples.cpp
				)

#libraries 
target_link_libraries( quic ${MPI_LIBRARIES})
target_link_libraries( quic ${Boost_LIBRARIES})