cmake_minimum_required(VERSION 2.8)
project (optimizationCode)

#########################
# GTest
#########################

add_subdirectory( ${GTEST_SOURCE_DIR} ${CMAKE_BINARY_DIR}/lib/google_test)

link_directories(${CMAKE_BIN_DIR}/lib)

enable_testing( )

include_directories (${GTEST_SOURCE_DIR} ${GTEST_SOURCE_DIR}/include)
include_directories (${CMAKE_SOURCE_DIR}/include)
include_directories (${CMAKE_SOURCE_DIR}/fitness_lib/include) 
include_directories (${Boost_INCLUDE_DIRS})
include_directories (${MPI_CXX_INCLUDE_PATH})
include_directories (${LIBSIVELAB_PATH}) 
include_directories (${CMAKE_SOURCE_DIR}/include)
include_directories (${CMAKE_SOURCE_DIR}/fitness_lib/include) 
include_directories (${ANTLR_INCLUDES})
include_directories (${QES_INCLUDE_DIRECTORIES})

#########################
# Unit Tests
#########################

add_subdirectory(opt_grammar)
add_subdirectory(utils)