cmake_minimum_required (VERSION 2.6)
project(opt_split)

set(LIBSIVELAB_PATH "${CMAKE_SOURCE_DIR}/../../../ " CACHE STRING "PATH to the directory where the libsivelan repository is located ")
INCLUDE_DIRECTORIES(${LIBSIVELAB_PATH})
LINK_DIRECTORIES(${LIBSIVELAB_PATH}/lib)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

set(BUILD_SHARED_LIBS FALSE)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(optfiledivider optfiledivider.cpp
samples.cpp
population_gen.cpp
namedOptParam.h  


)
target_link_libraries (optfiledivider sive-quicutil)
target_link_libraries (optfiledivider sive-util)


