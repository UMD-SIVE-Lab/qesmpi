cmake_minimum_required (VERSION 2.8) 
project (optimizationCode)


SET(LIBSIVELAB_PATH "${CMAKE_SOURCE_DIR}/../libsivelab")
SET(QUIC_DATA_PATH "${CMAKE_SOURCE_DIR}/../quicdata")
SET(QES_ROOT_DIR "${CMAKE_SOURCE_DIR}/quic_envsim")

#Build QES libraries only
set(BUILD_LIBS_ONLY "TRUE")

# 
#	CMake Module path for including additional CMake macros to make
#	finding libraries and other requirements easier.
#
SET( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH} )

message(STATUS "Adding quic_envsim directory...")
add_subdirectory(quic_envsim)

#do this for LSM or ${QES_INCLUDE_DIRECTORIES}
#
#	Make some directories that are used by the system
#	when dumping build files and simulation output
#
file( MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/programs" ) # for ptx files
file( MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/output" )
add_definitions( -DQES_OUTPUT_DIR="${CMAKE_CURRENT_BINARY_DIR}/output" ) # QES_OUTPUT_DIR
add_definitions( -DQES_PTX_DIR="${CMAKE_CURRENT_BINARY_DIR}/programs" ) # QES_PTX_DIR
##end

#Do not build QES code with -std=c++11, using c++11 for both nvcc and gcc throws "nullptr is undefined" compile error
#Found solution here: http://choorucode.com/2014/02/18/nullptr-is-undefined-error-with-nvcc/
#Setting compiler to use C++11
SET (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
SET (CMAKE_BUILD_TYPE Debug)

#fintess function shared library
message(STATUS "Adding fitness_lib directory...")
add_subdirectory(fitness_lib)

message(STATUS "Adding src directory...")
add_subdirectory(src)

message(STATUS "Adding test directory...")
#########################
# GTest
#########################
set(gtest_SOURCE_DIR lib/google_test)
add_subdirectory( ${gtest_SOURCE_DIR} )
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_subdirectory(test)
