cmake_minimum_required(VERSION 2.8)
project (optimizationCode)

#population generation test

#Add test cpp file
add_executable( test_population_gen test_population_gen.cpp )
#Link test executable against gtest & gtest_main
target_link_libraries( test_population_gen gtest gtest_main optm-utils)
add_test( NAME populationGenerationTest WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin COMMAND test_population_gen)


#population test
add_executable( test_population test_population.cpp )
#Link test executable against gtest & gtest_main
target_link_libraries( test_population gtest gtest_main optm-utils)
add_test( NAME populationTest WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin COMMAND test_population)

#opt params test
#Add test cpp file
add_executable( test_optParams test_optParams.cpp )
#Link test executable against gtest & gtest_main
target_link_libraries( test_optParams gmock gmock_main gtest gtest_main optm-utils)
add_test( NAME optParamsTest WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin COMMAND test_optParams)

#Add test cpp file
add_executable( test_directory test_directory.cpp )
#Link test executable against gtest & gtest_main
target_link_libraries( test_directory gmock gmock_main gtest gtest_main optm-utils)
target_link_libraries( test_directory sive-quicutil)
target_link_libraries( test_directory sive-util)

add_test( NAME directoryTest WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin COMMAND test_directory)

add_custom_command(TARGET test_directory PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_CURRENT_SOURCE_DIR}/testFile.txt ${CMAKE_BINARY_DIR}/bin)