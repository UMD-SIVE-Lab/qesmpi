cmake_minimum_required (VERSION 2.6) 
project (OptimizationFileAntlrParser)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/antlr)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/OptFileGrammarParser.cpp 
							${CMAKE_CURRENT_SOURCE_DIR}/OptFileGrammarParser.hpp
							${CMAKE_CURRENT_SOURCE_DIR}/OptFileGrammarLexer.cpp
							${CMAKE_CURRENT_SOURCE_DIR}/OptFileGrammarLexer.hpp
					COMMAND java -jar antlr-3.5.2-complete.jar OptFileGrammar.g
					DEPENDS OptFileGrammar.g
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				)

add_library(optfileparser OptFileGrammarParser.cpp
						  OptFileGrammarLexer.cpp
						  OptFileGrammar.g
						  )

SET(ANTLR_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/antlr PARENT_SCOPE)