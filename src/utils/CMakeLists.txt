cmake_minimum_required (VERSION 2.8) 
project (optimizationCode)

include_directories (${LIBSIVELAB_PATH}) 
include_directories(${CMAKE_SOURCE_DIR}/include)
add_library(optm-utils ArgumentParsing.cpp
						directory.cpp
						FitnessCache.cpp
						population.cpp
						solver.cpp
						population_gen.cpp
						#concentrationRedux.cpp
						opt_params.cpp
						)