cmake_minimum_required (VERSION 2.8) 
project (optimizationCode)

include_directories (${LIBSIVELAB_PATH}) 
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories (${CMAKE_SOURCE_DIR}/fitness_lib/include)
include_directories (${QES_INCLUDE_DIRECTORIES})

link_directories(${LIBSIVELAB_PATH})

add_library(mpi_framework job.cpp)
target_link_libraries (mpi_framework optm-utils)
target_link_libraries (mpi_framework sive-quicutil)
