cmake_minimum_required (VERSION 2.8) 
project (optimizationCode)

SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREADED OFF)
FIND_PACKAGE(Boost COMPONENTS filesystem system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

#
#	Include local source directories
#
add_definitions( -DQUIC_DATA_DIR="${QUIC_DATA_PATH}" ) # QUIC_DATA_DIR
add_definitions( -DQES_ROOT_DIR="${QES_PROJECT}" ) # QES_ROOT_DIR


#
#	Make some directories that are used by the system
#	when dumping build files and simulation output
#
file( MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/programs" ) # for ptx files
file( MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/output" )
add_definitions( -DQES_OUTPUT_DIR="${CMAKE_CURRENT_BINARY_DIR}/output" ) # QES_OUTPUT_DIR
add_definitions( -DQES_PTX_DIR="${CMAKE_CURRENT_BINARY_DIR}/programs" ) # QES_PTX_DIR


##QES include directories
include_directories (${LIBSIVELAB_PATH})
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/include/core")
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/include/gui")
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/include/lsm")
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/include/normalRender")
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/include/radiant")
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/include/transport")
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/include/viewfactor")
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/src/core")
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/lib/pugixml/src")
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/lib/utm-ll/include")
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/lib/plume")
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/lib/sunpos")
set (QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${QES_PROJECT}/lib/")


FIND_PACKAGE( OptiX REQUIRED )
set( QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}; ${OptiX_INCLUDE}" )
#
#	Cuda
#
FIND_PACKAGE(CUDA REQUIRED)
IF(CUDA_FOUND)
	set( QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS};${CUDA_INCLUDE_DIRS}" )
ELSE (CUDA_FOUND)
	MESSAGE(FATAL_ERROR "CUDA is required for compiling this project.  Please install CUDA or re-run cmake with -i to specify the CUDA directories.")
ENDIF(CUDA_FOUND)


set(QES_INCLUDE_DIRS "${QES_INCLUDE_DIRS}" CACHE STRING "QES include directories")

include_directories(${QES_INCLUDE_DIRS})

link_directories(${CMAKE_SOURCE_DIR}/lib/qes_lib)
link_directories(${OPTIX_PATH}/lib )
link_directories(${LIBSIVELAB_PATH}/lib)
#I dont like doing this but cant help
#Do not mess with the order libraries given here unless you want to get into trouble
set(QES_LIBRARIES libFormFactorTestLib.a
liblsmCPP.a
libQESGUI.a
libQESLSM.a
libQESNormalRender.a
libQESPTrackerLib.a
libQESRadiant.a
libQESLSMCuda.so
libQESCore.a
libQESCoreCuda.so
libQESTransport.a
libQESViewfactor.a
libQESWorkloadTestLib.a
libQPTurbulence.a
libutm-ll.a
)

MESSAGE(INFO ${QES_LIBRARIES})
MESSAGE(INFO ${Boost_LIBRARIES})
add_executable( simpleLSM simpleLSM.cpp )
target_link_libraries(simpleLSM ${QES_LIBRARIES})
target_link_libraries(simpleLSM ${Boost_LIBRARIES})
target_link_libraries(simpleLSM ${CUDA_LIBRARIES})
target_link_libraries(simpleLSM optix)
target_link_libraries(simpleLSM sive-cudautil)
target_link_libraries(simpleLSM sive-quicutil)
target_link_libraries(simpleLSM sive-util)
target_link_libraries(simpleLSM Gui)

