cmake_minimum_required (VERSION 2.8) 
project (optimizationCode)

#MPI 
FIND_PACKAGE(MPI REQUIRED)

#BOOST
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREADED OFF)
FIND_PACKAGE(Boost COMPONENTS mpi serialization REQUIRED)

#add sub directory
message(STATUS "Adding opt_grammar directory...")
add_subdirectory(opt_grammar)

message(STATUS "Adding utils directory...")
add_subdirectory(utils)

message(STATUS "Adding mpi_gpuplume directory...")
add_subdirectory(mpi_gpuplume)
add_subdirectory(mpi_qes_spf)


#include_directories
include_directories (${Boost_INCLUDE_DIRS})
include_directories (${MPI_CXX_INCLUDE_PATH})
include_directories (${LIBSIVELAB_PATH}) 
include_directories (${CMAKE_SOURCE_DIR}/include) 
include_directories (opt_grammar)
include_directories (opt_grammar/antlr)

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
add_executable(quic quic.cpp)

#libraries 
target_link_libraries (quic ${MPI_LIBRARIES})
target_link_libraries (quic ${Boost_LIBRARIES})
target_link_libraries (quic sive-quicutil)
target_link_libraries (quic sive-util) 
target_link_libraries (quic optfileparser)
target_link_libraries (quic mpi_gpuplume)
#target_link_libraries (quic mpi_qes_spf)
