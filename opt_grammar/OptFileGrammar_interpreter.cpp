#include <iostream>
#include "fstream"

#include "OptFileGrammarLexer.hpp"
#include "OptFileGrammarParser.hpp"

constexpr bool RUN_AS_INTERPRETER = false;
int main()
{
    OptFileGrammarLexer::InputStreamType input((ANTLR_UINT8 *) "", ANTLR_ENC_8BIT, 0, (ANTLR_UINT8 *) "userinput");

    std::string inputLine;
    ifstream fin("optTest8D.opt", std::ifstream::in);
    istream *input_stream = &fin;
    if (RUN_AS_INTERPRETER)
    {
        input_stream = &std::cin;
        std::cout << "Rias> ";
    }

    long line_number = 1;
    while (getline(*input_stream, inputLine))
    {
        trim(inputLine);
        if (inputLine.size() != 0)
        {
            input.reuse((ANTLR_UINT8 *) inputLine.c_str(), inputLine.length(), (ANTLR_UINT8 *) "userinput");
            OptFileGrammarLexer lexer(&input);
            OptFileGrammarParser::TokenStreamType token_stream(ANTLR_SIZE_HINT, lexer.get_tokSource());
            OptFileGrammarParser parser(&token_stream);
            parser.unit();
            if (lexer.error_in_lexer | parser.error_in_parser)
            {
                cout << "At line " << line_number<<endl<<endl;
            }
            else
            {

                std::cout << "Parsing line: " << inputLine << endl;
                std::cout << "Extracted info:" << std::endl;
                for (auto &itr : parser.each_line)
                {
                    std::cout << itr.first << " : " << itr.second << ", " << endl;
                }
                std::cout << std::endl;
            }
        }
        if (RUN_AS_INTERPRETER)
        {
            std::cout << "Rias>";
        }
        line_number++;
    }
}