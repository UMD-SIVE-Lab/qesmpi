cmake_minimum_required (VERSION 2.8) 
project (optimizationCode)

# 
#	CMake Module path for including additional CMake macros to make
#	finding libraries and other requirements easier.
#	Don't need this anymore seems to work with newer versions of CMake 
#	SET( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH} )

#
#	Include local source directories
#
add_definitions( -DQUIC_DATA_DIR="${QUIC_DATA_PATH}" ) # QUIC_DATA_DIR
add_definitions( -DQES_ROOT_DIR="${QES_ROOT_DIR}" ) # QES_ROOT_DIR

#
#	Make some directories that are used by the system
#	when dumping build files and simulation output
#
file( MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/programs" ) # for ptx files
file( MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/output" )
#end

include_directories (${QES_INCLUDE_DIRECTORIES})
include_directories (${CMAKE_SOURCE_DIR}/include)
include_directories (${CMAKE_SOURCE_DIR}/fitness_lib/include)
include_directories (${LIBSIVELAB_PATH}) 



##QES include directories
add_library ( mpi_simpleLSM simpleLSM_job.cpp )
target_link_libraries( mpi_simpleLSM QESLSM QESViewfactor )
target_link_libraries (mpi_simpleLSM optm-utils)
target_link_libraries (mpi_simpleLSM dl)
target_link_libraries (mpi_simpleLSM mpi_framework)
target_link_libraries (mpi_simpleLSM ${Boost_LIBRARIES})
