all: OptFileGrammar

OptFileGrammarLexer.cpp OptFileGrammarParser.cpp: OptFileGrammar.g
	java -jar antlr-3.5.2-complete.jar OptFileGrammar.g

OptFileGrammar: OptFileGrammar_interpreter.cpp OptFileGrammarLexer.cpp OptFileGrammarParser.cpp
	g++ -g -std=c++11 -I  `pwd`/antlr -o OptFileGrammar OptFileGrammar_interpreter.cpp OptFileGrammarLexer.cpp OptFileGrammarParser.cpp

clean:
	rm -f OptFileGrammarLexer.cpp OptFileGrammarParser.cpp OptFileGrammar OptFileGrammarLexer.hpp OptFileGrammarParser.hpp
