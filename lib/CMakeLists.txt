cmake_minimum_required(VERSION 2.6)

project (fitness_function)
SET (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")


add_library (fitness SHARED fitness_function.cpp)

INCLUDE_DIRECTORIES (..)

