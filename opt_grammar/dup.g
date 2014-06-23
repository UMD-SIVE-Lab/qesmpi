grammar OptFileGrammar;

options {
  language = Cpp;
}

@parser::includes {
#include <iostream>
  #include "OptFileGrammarLexer.hpp"
  #include "boost/algorithm/string.hpp"
  #include "map"
  #include "string"
  using namespace std;
  using namespace boost;
}

@lexer::traits {
class OptFileGrammarLexer;
  class OptFileGrammarParser;
  typedef antlr3::Traits<OptFileGrammarLexer, OptFileGrammarParser> OptFileGrammarLexerTraits;
  typedef OptFileGrammarLexerTraits OptFileGrammarParserTraits;
}

@context {

}

@members {

}

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
    | '\r'
    | '\n'
  )
  
  {
   $channel=HIDDEN;
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


unit returns [map<string, string> ret]
  :
  assignment 
  {
   std::cout<<"Assignment statement: "<<$assignment.text;
   ret = map<string, string>();
  }
  | comment 
  {
  //ret = comment;
   std::cout<<"comment statement: "<<$comment.text;
  }
  ;
  
assignment //returns [map<string, string> ret]
  :
  WS* lval=lvalue WS* '=' WS* rval=rvalue WS* (comment?)
  {
    std::cout<<$lval.text;
    std::cout<<$rval.text;
    //ret["lval"] = $lval.text;
    //ret["rval"] = $rval.text;
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
     lval["lval"] = $constant.text;
     lval["type"] = "control";
    }
  | local_variable 
     {
      //do these exist at all? Yup they do exists see adityas code he parses them
      lval["lval"] = $local_variable.text;
      lval["type"] = "project_variable";
     }
  | member_variable 
    {
     lval["lval"] = $member_variable.text;
     lval["type"] = "project_variable";
    }
  ;
/*
 rvalue can be a number (int, float), string, set, range or a simple expression  
*/


rvalue returns [std::map<string, string> rval]
  :
  num //Number- find how to capture lexer variables
   {
    rval["rval"] = $num.text;
    rval["type"] = "number";
   }
  | str 
  {
  string temp  = $str.text;
  trim_if(temp, is_any_of("'"));
  rval["rval"] = temp;
  rval["type"] = "string";
  }
  | set 
  {
   string temp  = $set.text;
   trim_if(temp, is_any_of("[]"));
   rval["rval"] = temp;
   rval["type"] = "set";
  }
  | range 
  {
   string temp  = $range.text;
   trim_if(temp, is_any_of("[]"));
   rval["rval"] = temp;
   rval["type"] = "range";
  }
 | atomExp 
 {
  rval["rval"] = $atomExp.text;
  rval["type"] = "atomExp";
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
  lvalue
  (
    '+'
    | '-'
  )
  rvalue
  ;

set
  :
  '[' (Number WS)+ ']'
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
  '\''
  ~(
    '\r'
    | '\n'
    | '\''
   )*
  '\''
  ;
