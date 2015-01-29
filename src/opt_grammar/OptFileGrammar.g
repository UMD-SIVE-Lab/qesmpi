grammar OptFileGrammar;

options {
  language = Cpp;
}

@lexer::includes {
#include <iostream>
  #include "antlr3exception.hpp"
  #include "boost/algorithm/string.hpp"
  using namespace std;
  using namespace boost;
}

@parser::includes {
#include <iostream>
  #include "antlr3exception.hpp"
  #include "OptFileGrammarLexer.hpp"
  #include "boost/algorithm/string.hpp"
  #include "map"
  #include "string"
  #include "vector"
  using namespace std;
  using namespace boost;
}

@lexer::traits {
class OptFileGrammarLexer;
  class OptFileGrammarParser;
  typedef antlr3::Traits<OptFileGrammarLexer, OptFileGrammarParser> OptFileGrammarLexerTraits;
  typedef OptFileGrammarLexerTraits OptFileGrammarParserTraits;
}

@lexer::context {
bool error_in_lexer = false;
StringStreamType  err_stream;
void displayRecognitionError( ANTLR_UINT8** , ExceptionBaseType* ex);
}

@lexer::members {
void OptFileGrammarLexer::displayRecognitionError(ANTLR_UINT8** tokens, Lexer::ExceptionBaseType* ex){
            /*
              error_in_lexer = true;
              stringstream ss;
              exception->displayRecognitionError(tokens, ss);
              string message = ss.str();
              trim(message);
              ss.str("");
              
              ss<<message<<" at pos: "<<(exception->get_charPositionInLine())+1;
              message = ss.str();
              trim_if(message, is_any_of(":"));
              trim(message);
              cerr<<"Lexer ERROR: "<<message<<endl;
              //_/\_ antlr documentation
              
              cerr<<"get_message()"<<exception->get_message()<<endl;
              cerr<<"get_streamName()"<<exception->get_streamName()<<endl;
              cerr<<"get_index()"<<exception->get_index()<<endl;
              cerr<<"get_token()"<<exception->get_token()<<endl;
              cerr<<"get_expecting()"<<exception->get_expecting()<<endl;
              cerr<<"get_expectingSet()"<<exception->get_expectingSet()<<endl;
              cerr<<"get_node()"<<exception->get_node()<<endl;
              cerr<<"get_c()"<<exception->get_c()<<endl;
              cerr<<"get_line()"<<exception->get_line()<<endl;
              cerr<<"get_state()"<<exception->get_state()<<endl;
              cerr<<"get_ruleName()"<<exception->get_ruleName()<<endl;
              cerr<<"get_charPositionInLine()"<<exception->get_charPositionInLine()<<endl;
              cerr<<"get_input()"<<exception->get_input()<<endl;
              cerr<<"get_message()"<<exception->get_message()<<endl;
          */
              err_stream.clear();        
              err_stream<< "Lexer error: " <<ex->get_message() << " at offset "
                   << ex->get_charPositionInLine()+1;     
              error_in_lexer = true;    
              
              //cerr<<err_stream.str()<<endl;   
          }
}
//this goes into header file

@context {
std::map<string, string> each_line;
  bool error_in_parser = false;
  stringstream errtext;

  void displayRecognitionError(ANTLR_UINT8** tokens, Parser::ExceptionBaseType* exception);
}
//this goes into cpp file

@members {
void OptFileGrammarParser::displayRecognitionError(ANTLR_UINT8** tokens, Parser::ExceptionBaseType* exception){
    if(tokens!=NULL){
     /*
      stringstream ss;
      exception->displayRecognitionError(tokens, ss);
      string message = ss.str();
      trim(message);
      ss.str("");
      
      ss<<message<<" at pos: "<<(exception->get_charPositionInLine())+1;
      message = ss.str();
      trim_if(message, is_any_of(":"));
      trim(message);
      cerr<<"Parsing ERROR: "<<message<<endl;
      //_/\_ antlr documentation
      cerr<<"get_message()"<<exception->get_message()<<endl;
      cerr<<"get_streamName()"<<exception->get_streamName()<<endl;
      cerr<<"get_index()"<<exception->get_index()<<endl;
      cerr<<"get_token()"<<exception->get_token()<<endl;
      cerr<<"get_expecting()"<<exception->get_expecting()<<endl;
      cerr<<"get_expectingSet()"<<exception->get_expectingSet()<<endl;
      cerr<<"get_node()"<<exception->get_node()<<endl;
      cerr<<"get_c()"<<exception->get_c()<<endl;
      cerr<<"get_line()"<<exception->get_line()<<endl;
      cerr<<"get_state()"<<exception->get_state()<<endl;
      cerr<<"get_ruleName()"<<exception->get_ruleName()<<endl;
      cerr<<"get_charPositionInLine()"<<exception->get_charPositionInLine()<<endl;
      cerr<<"get_input()"<<exception->get_input()<<endl;
      cerr<<"get_message()"<<exception->get_message()<<endl;
      */
        // See if there is a 'filename' we can use
        //
        
        error_in_parser = true;
        errtext.clear();
        errtext << "Parser Error: " << this->get_rec()->get_state()->get_exception()->get_message();

        // Prepare the knowledge we know we have
        //
        const CommonTokenType* theToken   = this->get_rec()->get_state()->get_exception()->get_token();
        StringType ttext      = theToken->toString();

        errtext << ", at offset , "
            << this->get_rec()->get_state()->get_exception()->get_charPositionInLine();
        if  (theToken != NULL)
        {
          if (theToken->get_type() == CommonTokenType::TOKEN_EOF)
          {
            errtext << ", at <EOF>\n";
          }
          else
          {
            // Guard against null text in a token
            //
            errtext << "\n    near " << ( ttext.empty()
                            ? "<no text for the token>" : ttext ) << "\n";
          }
        }

        //cerr<<errtext.str();

    }
  }
}

//check what EOF is in antlr

/* A number: can be an integer value, or a decimal value */

Number
  :
  ('0'..'9')+ ('.' ('0'..'9')+)?
  ;

num
  :
  Number
  ;
/* We're going to ignore all white space characters */

WS
  :
  (
    ' '
    | '\t'
  )
  
  {
   //$channel=HIDDEN; //we need white spaces to differentiate sperate const keyword from the variable name
  }
  ;

Identifier
  :
  (
    'a'..'z'
    | 'A'..'Z'
    | '_'
  )
  (
    'a'..'z'
    | 'A'..'Z'
    | '_'
    | '0'..'9'
  )*
  ;
/*
  Basic unit in opt file language is assignment
*/


unit
  :
  (
    assignment 
              {
               ////std::cout<<"Assignment statement: "<<$assignment.text;
              }
    | comment 
             {
              //std::cout<<"comment statement: "<<$comment.text;
                            //each_line["type"] = "comment";
             }
  )
  EOF
  ;

assignment
  :
  WS* lval=lvalue WS* '=' WS* rval=rvalue WS* (comment?)
  {
    /*Dont even use '//' for commenting c++ code blocks in antlr it might end up as a multiline statement like the below
      std::cout<<"lvalue = "<<$lvalue.text<<endl<<"rvalue="<<$rvalue.text<<endl;
      Generated code:
                    //std::cout<<"lvalue = "<<(this->get_strstream()->toStringTT(lval.start, lval.stop))
                <<endl<<"rvalue="<<(this->get_strstream()->toStringTT(rval.start, rval.stop))
                <<endl;
    */
  }
  ;

comment
  :
  '//' .*
  ;
/* 
 lvalue can be a control variable (respresented as constants) or local variable or a member of a class 
*/


lvalue returns [std::map<string, string> lval]
  :
  constant 
          {
           string temp =  $constant.text;
                           erase_all(temp, "const");
                           trim(temp);
                           each_line["lval"] = temp;
                           each_line["lval_type"] = "control";
          }
  | local_variable 
                  {
                    cout<<$local_variable.text;
                   //do these exist at all? Yup they do exists see adityas code he parses them
                                    each_line["lval"] = $local_variable.text;
                                    each_line["lval_type"] = "project_variable";
                  }
  | member_variable 
                   {
                      cout<<$member_variable.text;
                      each_line["lval"] = $member_variable.text;
                                    each_line["lval_type"] = "project_variable";
                   }
  ;
/*
 rvalue can be a number (int, float), string, set, range or a simple expression  
*/


rvalue returns [std::map<string, string> rval]
  :
  str 
     {
      string temp  = $str.text;
                trim_if(temp, is_any_of("'"));
                each_line["rval"] = temp;
                each_line["rval_type"] = "string";
                //cout<<"string";
     }
  | set 
       {
        string temp  = $set.text;
                  trim_if(temp, is_any_of("[]"));
                  each_line["rval"] = temp;
                  each_line["rval_type"] = "set";
                  //cout<<"set";
       }
  | range 
         {
          string temp  = $range.text;
                     trim_if(temp, is_any_of("[]"));
                     std::vector<string> temp2;
                     split(temp2, temp, is_any_of(":"));
                     each_line["min"]  = temp2[0];
                     each_line["step"] = temp2[1];
                     each_line["max"]  = temp2[2];
                     each_line["rval_type"] = "range";
                     //cout<<"range";
         }
  | atomExp 
           {
           //antlr executes this code even if there is an error in parsing a semantic rule completely
           //so place a check if there is only partial match in parsing
           //like in case of input a=b' it still executes the below code block
           //add this check to all parsing rules 
           //One more time antlr _/\_
           //may be there is a better way of doing things
           if(!error_in_parser){
                       string temp = $atomExp.text;
                       std::vector<string> temp2;
                       split(temp2, temp, is_any_of("+-"));
                       each_line["left_opd"] = temp2[0];
                       each_line["op"] = temp[temp2[0].size()];
                       each_line["right_opd"] = temp2[1];
                       stringstream ss;
                       ss<<"atomExp"<<int(drand48()*100);//I dont think there will be more than 100 expressions
                       each_line["rval_type"] = ss.str();
                       //cout<<"atomexp";
                       }
           }
  | num //Number- find how to capture lexer variables
  {
        each_line["rval"] = $num.text;
                each_line["rval_type"] = "number";
                //cout<<"number";
       }
  ;
/*
  local variable is just an identifier
*/


local_variable
  :
  Identifier
  ;
/*
  constant variables are control variables that specify various things like solvers etc
*/


constant
  :
  'const' WS+ Identifier
  ;
/*
  member variable are of the form classname followed by member names which can be 
  either array member (array[2]) or a name but the last dot should always be followed by a name
  eg: quBuildings.buildings[0].xfo = 23
           ^          ^         ^
           |          |         |
      classname   array_member  |
                               name
*/


member_variable
  :
  Identifier ('.' vector)* '.' Identifier
  ;

atomExp
  :
  (num | member_variable | local_variable)
  WS*(
    '+'
    | '-'
  ) WS*
  (num | member_variable | local_variable)
  ;

set
  :
  '[' (Number WS*)+ ']'
  ;

range
  :
  '[' Number ':' Number ':' Number ']'
  ;

vector
  :
  Identifier '[' Number ']'
  ;

str
  :
  String;

String:
  '\''
  ~(
    '\r'
    | '\n'
    | '\''
   )*
  '\''
  ;
