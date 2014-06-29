grammar grammartesting;

options {
  language = Java;
}

@header {
package com.amibee;
}

@lexer::header {
package com.amibee;
}

//reserve words
Print    : 'disp';
Function : 'function';
If       : 'if';
Else     : 'else';
For      : 'for';
//Consider adding while loop 

//since there will be math NaN is possible
NaN      : 'NaN';

Or       : 'or';
And      : 'and';
Not      : 'not';

Equals   : '==';
NEquals  : '~=';
GTEquals : '>=';
LTEquals : '<=';
Pow      : '^';
GT       : '>';
LT       : '<';
//reserve words
parse
  :
  (t=. 
      {
       System.out.printf("text: \%-7s  type: \%s \n",  
                                                                                                                                         $t.text, tokenNames[$t.type]);
      })+ EOF
  ;

Bool
  :
  'true'
  | 'false'
  ;

Number
  :
  Int ('.' Digit*)?
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
    | Digit
  )*
  ;

fragment
Int
  :
  '1'..'9' Digit*
  | '0'
  ;

fragment
Digit
  :
  '0'..'9'
  ;

fragment
LineBreak
  :
  '\r' '\n'?
  | '\n'
  ;

fragment
Escapesequences
  :
  '\\'
  (
    'r'
    | 'n'
    | 't'
    | '\\'
  )
  ;

Space
  :
  (
    ' '
    | '\t'
    | '\r'
    | '\n'
    | '\u000C'
  )
  
  {
   //{skip();} or {$channel=HIDDEN;} cause lexer rules to be skipped (or hidden) from parser rules, not from lexer rules.
   $channel = HIDDEN;
  }
  ;

Comment
  :
  //A block comment is '%{' followed by any character followed by '%}'
  '%{' (.*) '%}' 
                {
                 //skip();
                }
  //A single line comment is '%' followed zero or one sentence.
  //That should not begin with '}'
  // and the rest of the sentence should not have new line characters ('\r' '\n')
  | '%'
  (
    ~(
      '{'
      | '\r'
      | '\n'
     )
    ~(
      '\r'
      | '\n'
     )*
  )?
  
  {
   //skip();
  }
  ;
//A string is enclosed in single quotes
//It conatuins any characters other than a single quote, single slash, line breaks( Whole string must be in one line)
//To get a single quote use two consecutive quotes eg. 'matlab''s' -> matlab's
//escape sequences \r,\n,\t, \\ are allowed

String
@after {
/*
//this does not work when the string is a member of the vector in which case getText returns all the vector matched till the end of the string
//eg. [1 2 [1 2] ''] -> "1 2 [1 2] '"
//replace single quote delimiting with double quotes
String temp = getText();
System.out.println(temp);
//escape double quotes  as c++ uses them as delimiters
temp = temp.replace("\"", "\\\"");
//change delimiters ' as "
temp = temp.substring(1, temp.length() - 1);
temp = "\"" + temp + "\"";
//replace '' as '
temp = temp.replaceAll("''", "\\\\'");//args are regular expressions
setText(temp);
*/
}
  :
  '\''
  (
    ~(
      '\''
      | '\\'
      | '\r'
      | '\n'
     )
    | '\'' '\''
    | (Escapesequences)
  )*
  '\''
  ;

fragment
Vectordelimiters
  :
  ' '
  | '\t'
  | ','
  ;

fragment
Vectormembers
  :
  Number
  | String
  | Vector
  ;

Vector
  :
  '[' (Vectormembers)? (Vectordelimiters+ (Vectormembers))* ']'
  ;

Range
  :
  '[' Number ':' Number ':' Number ']'
  ;
