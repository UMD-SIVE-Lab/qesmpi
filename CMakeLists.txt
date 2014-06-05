cmake_minimum_required (VERSION 2.6) 
project (optimizationCode)

SET(LIBSIVELAB_PATH "${CMAKE_SOURCE_DIR}/../libsivelab" CACHE STRING "Path to the directory where the libsivelab repository is located (assumes code is built into a directory named 'build').")

SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
IF (NOT Boost_FOUND)
	message ("Boost nor found")

ENDIF()

include_directories(${Boost_INCLUDE_DIRS})

add_executable(quic quic.cpp
					ArgumentParsing.cpp
					master.cpp
					FitnessCache.cpp
					population_gen.cpp
					samples.cpp
				)
