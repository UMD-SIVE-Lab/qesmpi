//Following are the valid strings that can be parsed by "OptFileGrammar.g" grammar
//Some extra strings like arithmetic operations are also supported by "OptFileGrammar.g"
// but their functionality is not used yet.
#include "OptFileGrammarLexer.hpp"
#include "OptFileGrammarParser.hpp"

#include "gtest/gtest.h"

#include "sstream"

using namespace std;

class OptFileParseTest: public testing::Test {
        
    public:
        stringstream OptfileContents;
        map < string, map<string, string>> optParams;
        string error;
        int readOptimizationFile()
        {
         
            OptFileGrammarLexer::InputStreamType input((ANTLR_UINT8 *) "", ANTLR_ENC_8BIT, 0, (ANTLR_UINT8 *) "userinput");
            std::string inputLine;
            istream *input_stream = &OptfileContents;

            long line_number = 1;
            while (getline(*input_stream, inputLine))
            {
                if (inputLine.size() != 0)
                {

                    input.reuse((ANTLR_UINT8 *) inputLine.c_str(), inputLine.length(), (ANTLR_UINT8 *) "userinput");
                    OptFileGrammarLexer lexer(&input);
                    OptFileGrammarParser::TokenStreamType token_stream(ANTLR_SIZE_HINT, lexer.get_tokSource());
                    OptFileGrammarParser parser(&token_stream);
                    parser.unit();
                    if (lexer.error_in_lexer || parser.error_in_parser)
                    {
                        error.clear();
                        error = lexer.err_stream.str()+parser.errtext.str();
                        return -1;
                    }
                    else
                    {
                        for (auto &itr : parser.each_line){
                            cout<<itr.first<<" : "<<itr.second<<endl;
                        }
                        if(parser.each_line.size()!=0)
                            optParams[parser.each_line["lval"]] = parser.each_line;
                    }
                }
                line_number++;
            }
            return 0;
        }
};
/*
const job_type = 'lsm'\n"
                   <<"//dummy = [1:1:100]\n"
                   <<"const seed = 9999\n"
                   <<"quBuildings.buildings[0].xfo = [23 24]\n"
                   <<"quBuildings.buildings[0].yfo = [37.0:1.0:40.0]\n"
                   <<"//quBuildings.buildings[4].xfo = 23 + quBuildings.buildings[0].xfo\n";

*/
TEST_F(OptFileParseTest, comment) {
    OptfileContents<<"//This is a comment";
    ASSERT_NE(-1, readOptimizationFile())<<"Expected error: : "<<error;
    //comments are not included in opt params, they are ignored.
    ASSERT_EQ(0, optParams.size());
    SUCCEED();
}

TEST_F(OptFileParseTest, number_assignment) {
    OptfileContents<<" a=1";
    ASSERT_NE(-1, readOptimizationFile())<<"Expected error: : "<<error;
    //there must be one entry in optparams
    ASSERT_EQ(optParams.size(), 1);
    auto current_map = optParams["a"];
    
    ASSERT_EQ("a", current_map["lval"]);
    ASSERT_EQ("project_variable", current_map["lval_type"]);
    ASSERT_EQ("1", current_map["rval"]);
    ASSERT_EQ("number", current_map["rval_type"]);
}

TEST_F(OptFileParseTest, string_assignment) {
    string str = "str='../this/is/a/path'";
    OptfileContents<<str;
    ASSERT_NE(-1, readOptimizationFile())<<"Expected error: : "<<error;
    //there must be one entry in optparams
    ASSERT_EQ(optParams.size(), 1);
    auto current_map = optParams["str"];
    
    ASSERT_EQ("str", current_map["lval"]);
    ASSERT_EQ("project_variable", current_map["lval_type"]);
    ASSERT_EQ("../this/is/a/path", current_map["rval"]);
    ASSERT_EQ("string", current_map["rval_type"]);
}

//There are two type of variables project variables and control variables
//Project variables specify changes to parameters etc
//Control variables specify things that all simulations need like
//base project path, type of solver to use etc
TEST_F(OptFileParseTest, control_variable) {
    string str = "const str='../this/is/a/path'";
    OptfileContents<<str;
    ASSERT_NE(-1, readOptimizationFile())<<"Expected error: : "<<error;
    //there must be one entry in optparams
    ASSERT_EQ(optParams.size(), 1);
    auto current_map = optParams["str"];
    ASSERT_EQ("str", current_map["lval"]);
    
    ASSERT_EQ("control", current_map["lval_type"]);
    
    ASSERT_EQ("../this/is/a/path", current_map["rval"]);
    ASSERT_EQ("string", current_map["rval_type"]);
}

TEST_F(OptFileParseTest, empty_input) {
    string str = "";
    OptfileContents<<str;
    ASSERT_NE(-1, readOptimizationFile())<<"Expected error: : "<<error;
    //there must be one entry in optparams
    ASSERT_EQ(optParams.size(), 0);
}

TEST_F(OptFileParseTest, set_assignment){
    //beware this is not a mathematical set it is an array
    //TODO: change the naming to array someday
    string str = "setVar = [10 11 12]";
    OptfileContents<<str;
    ASSERT_NE(-1, readOptimizationFile())<<"Expected error: : "<<error;
    //there must be one entry in optparams
    ASSERT_EQ(optParams.size(), 1);
    auto current_map = optParams["setVar"];
    ASSERT_EQ("setVar", current_map["lval"]);
    ASSERT_EQ("project_variable", current_map["lval_type"]);
    string vecString = current_map["rval"];
    ASSERT_EQ("set", current_map["rval_type"]);
    string  expectedVecString= "10 11 12";
    ASSERT_EQ(expectedVecString.length(), vecString.length());
    ASSERT_EQ(expectedVecString, vecString);
}

TEST_F(OptFileParseTest, range_assignment){
    string str = "rangeVar = [1:3:10]";
    OptfileContents<<str;
    ASSERT_NE(-1, readOptimizationFile())<<"Expected error: : "<<error;
    //there must be one entry in optparams
    ASSERT_EQ(optParams.size(), 1);
    auto current_map = optParams["rangeVar"];
    ASSERT_EQ("rangeVar", current_map["lval"]);
    ASSERT_EQ("project_variable", current_map["lval_type"]);
    ASSERT_EQ("range", current_map["rval_type"]);
    string minExpected = "1";
    string stepExpected = "3";
    string maxExpected = "10";
    ASSERT_EQ(minExpected, current_map["min"]);
    ASSERT_EQ(stepExpected, current_map["step"]);
    ASSERT_EQ(maxExpected, current_map["max"]);
}

TEST_F(OptFileParseTest, vector_variable_missing_index){
    string str = "object[] = 20";
    OptfileContents<<str;
    //this test should result in a parse error
    ASSERT_EQ(-1, readOptimizationFile())<<"Expected error: : "<<error;
    
}

TEST_F(OptFileParseTest, vector_variable){
    string str = "object[10] = 20";
    //There are no stand alone vector variables in this language.
    //A vector must always be followed by a '.identifier' e.g.object[1].member[0] 
    //It's funny!
    OptfileContents<<str;
    ASSERT_EQ(-1, readOptimizationFile())<<"Expected error: : "<<error;
}

TEST_F(OptFileParseTest, member_variable){
    string str = "object.member = 20";
    OptfileContents<<str;
    ASSERT_NE(-1, readOptimizationFile())<<"Expected error: : "<<error;
    //there must be one entry in optparams
    ASSERT_EQ(optParams.size(), 1);
    auto current_map = optParams["object.member"];
    ASSERT_EQ("object.member", current_map["lval"]);
    ASSERT_EQ("project_variable", current_map["lval_type"]);
    ASSERT_EQ("number", current_map["rval_type"]);
    ASSERT_EQ("20", current_map["rval"]);
    
}

TEST_F(OptFileParseTest, last_member_variable_is_array){
    string str = "object.member[0] = 20";
    OptfileContents<<str;
    //There are no stand alone vector variables in this language.
    //A vector must always be followed by a '.identifier' e.g.object[1].member[0] 
    //this test should result in a parse error
    ASSERT_EQ(-1, readOptimizationFile())<<"Expected error: : "<<error;
}

TEST_F(OptFileParseTest, invalid_memeber_variable_of_array){
    string str = "quBuildings[0].xfo = [23 24]";
    OptfileContents<<str;
    //this test should result in a parse error
    //member variables must begin with an identifier valid
    //eg. quBuildings.buildings[0].xfo = [23 24]
    //this test should result in a parse error
    ASSERT_EQ(-1, readOptimizationFile())<<"Expected error: : "<<error;
}

TEST_F(OptFileParseTest, memeber_variable_of_array){
    string str = "quBuildings.buildings[0].xfo = 23";
    OptfileContents<<str;
    ASSERT_NE(-1, readOptimizationFile())<<"Expected error: : "<<error;
    //there must be one entry in optparams
    ASSERT_EQ(optParams.size(), 1);
    auto current_map = optParams["quBuildings.buildings[0].xfo"];
    
    ASSERT_EQ("quBuildings.buildings[0].xfo", current_map["lval"]);
    ASSERT_EQ("project_variable", current_map["lval_type"]);
    ASSERT_EQ("23", current_map["rval"]);
    ASSERT_EQ("number", current_map["rval_type"]);
}

//test 
