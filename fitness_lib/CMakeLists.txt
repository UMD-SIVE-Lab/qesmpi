cmake_minimum_required(VERSION 2.6)

project (optimizationCode)
SET(LIBSIVELAB_PATH "${CMAKE_SOURCE_DIR}/../libsivelab")


add_library (fitness SHARED fitness_function.cpp)

include_directories (${LIBSIVELAB_PATH})
include_directories (${CMAKE_SOURCE_DIR}/include)
include_directories (${Boost_INCLUDE_DIRS})
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories (${QES_INCLUDE_DIRECTORIES})


