cmake_minimum_required(VERSION 2.6)

project (optimizationCode)

add_library (fitness SHARED fitness_function.cpp)

INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}/include)

