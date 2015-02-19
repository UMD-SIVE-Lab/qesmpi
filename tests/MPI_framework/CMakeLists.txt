cmake_minimum_required(VERSION 2.8)
project (optimizationCode)

#job class test
#Add test cpp file

add_executable( test_job test_job.cpp )
#Link test executable against gtest & gtest_main
target_link_libraries( test_job mpi_framework optm-utils )
target_link_libraries( test_job gtest gtest_main)
target_link_libraries( test_job sive-quicutil)
target_link_libraries( test_job sive-util)
target_link_libraries( test_job QESLSM QESViewfactor )
target_link_libraries( test_job ${Boost_LIBRARIES})


add_test( NAME JobTest WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin COMMAND test_job)

add_custom_command(TARGET test_job PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/2by2_q572_270 ${CMAKE_BINARY_DIR}/resources/2by2_q572_270)