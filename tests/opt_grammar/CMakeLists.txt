cmake_minimum_required(VERSION 2.8)
project (optimizationCode)

#Add test cpp file
add_executable( test_optGrammarTest test_opt_grammar.cpp )
#Link test executable against gtest & gtest_main
target_link_libraries( test_optGrammarTest gtest gtest_main optfileparser)
add_test( NAME optGrammarTest WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin COMMAND test_optGrammarTest)
