cmake_minimum_required (VERSION 2.8) 
project (optimizationCode)

#Matt has his own version of libsivelab I am also using that since using a seperate once
#is resulting in naming colissions. 
SET(LIBSIVELAB_PATH "${CMAKE_SOURCE_DIR}/lib/quic_envsim/lib/libsivelab")
SET(QUIC_DATA_PATH "${CMAKE_SOURCE_DIR}/../quicdata")
SET(QES_ROOT_DIR "${CMAKE_SOURCE_DIR}/../quic_envsim")
SET(GMOCK_SOURCE_DIR "${CMAKE_SOURCE_DIR}/lib/gmock")
SET(GTEST_SOURCE_DIR "${GMOCK_SOURCE_DIR}/gtest")

#Build QES libraries only
set(BUILD_LIBS_ONLY "TRUE")
#Build unit tests
set(BUILD_TESTS "TRUE")

#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

#Setting compiler to use C++11
SET (DEFAULT_CXX_FLAGS ${CMAKE_CXX_FLAGS})
SET (CMAKE_CXX_FLAGS "${DEFAULT_CXX_FLAGS} -std=c++11")
SET (CMAKE_BUILD_TYPE Debug)
message(STATUS "###Adding lib directory###")
add_subdirectory(lib)

#this include directories for quic_envsim 
#the variable is set in lib/quic_envsim with PARENT_SCOPE
#setting it here again so that it is visible in sibling directories of lib i.e., src, fitness_lib, test 
set(QES_INCLUDE_DIRECTORIES ${QES_INCLUDE_DIRECTORIES})

#need for quic_envsim include files
add_definitions( -DQES_OUTPUT_DIR="${CMAKE_CURRENT_BINARY_DIR}/output" ) # QES_OUTPUT_DIR
add_definitions( -DQES_PTX_DIR="${CMAKE_CURRENT_BINARY_DIR}/programs" ) # QES_PTX_DIR

message(STATUS "###Adding src directory###")
add_subdirectory(src)
SET(ANTLR_INCLUDES ${ANTLR_INCLUDES})

#fintess function shared library
message(STATUS "###Adding fitness_lib directory###")
add_subdirectory(fitness_lib)

if(BUILD_TESTS)
	enable_testing( )
	message(STATUS "###Adding test directory###")
	add_subdirectory(tests)
endif()
