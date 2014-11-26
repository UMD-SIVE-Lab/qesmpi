cmake_minimum_required (VERSION 2.8) 
project (optimizationCode)

include_directories (${LIBSIVELAB_PATH}) 

include_directories(${CMAKE_SOURCE_DIR}/include)
add_library(mpi_gpuplume gpu_plume_job.cpp)

target_link_libraries (mpi_gpuplume optm-utils)
target_link_libraries (mpi_gpuplume dl)
target_link_libraries (mpi_gpuplume mpi_framework)
target_link_libraries (mpi_gpuplume mpi_framework)