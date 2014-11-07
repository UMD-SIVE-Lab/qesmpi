cmake_minimum_required (VERSION 2.8) 
project (optimizationCode)

#
#	Include local source directories
#
set( QUIC_DATA_PATH "/res/genusis/quicdata" CACHE FILEPATH "The path to your QUIC Data repository" )
add_definitions( -DQUIC_DATA_DIR="${QUIC_DATA_PATH}" ) # QUIC_DATA_DIR
add_definitions( -DQES_ROOT_DIR="${QES_ROOT_DIR}" ) # QES_ROOT_DIR


#
#	Make some directories that are used by the system
#	when dumping build files and simulation output
#
file( MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/programs" ) # for ptx files
file( MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/output" )
add_definitions( -DQES_OUTPUT_DIR="${CMAKE_CURRENT_BINARY_DIR}/output" ) # QES_OUTPUT_DIR
add_definitions( -DQES_PTX_DIR="${CMAKE_CURRENT_BINARY_DIR}/programs" ) # QES_PTX_DIR

INCLUDE_DIRECTORIES( ${QES_INCLUDE_DIRECTORIES} )

##QES include directories
add_executable( simpleLSM simpleLSM.cpp )
target_link_libraries( simpleLSM QESLSM QESViewfactor )

