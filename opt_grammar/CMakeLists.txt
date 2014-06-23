cmake_minimum_required (VERSION 2.6) 
project (OptimizationFileAntlrParser)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/antlr)

add_library(optfileparser OptFileGrammarParser.cpp
						  OptFileGrammarLexer.cpp)

