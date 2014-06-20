/** \file
 *  This C++ source file was generated by $ANTLR version 3.5.2
 *
 *     -  From the grammar source file : OptFileGrammar.g
 *     -                            On : 2014-06-18 15:28:29
 *     -                 for the lexer : OptFileGrammarLexerLexer
 *
 * Editing it, at least manually, is not wise.
 *
 * C++ language generator and runtime by Gokulakannan Somasundaram ( heavy lifting from C Run-time by Jim Idle )
 *
 *
*/
// [The "BSD license"]
// Copyright (c) 2005-2009 Gokulakannan Somasundaram, ElectronDB
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/* -----------------------------------------
 * Include the ANTLR3 generated header file.
 */
#include    "OptFileGrammarLexer.hpp"
/* ----------------------------------------- */




/** String literals used by OptFileGrammarLexer that we must do things like MATCHS() with.
 *  C will normally just lay down 8 bit characters, and you can use L"xxx" to
 *  get wchar_t, but wchar_t is 16 bits on Windows, which is not UTF32 and so
 *  we perform this little trick of defining the literals as arrays of UINT32
 *  and passing in the address of these.
 */
static ANTLR_UCHAR	lit_1[]  = { 0x2F, 0x2F,  antlr3::ANTLR_STRING_TERMINATOR};
static ANTLR_UCHAR	lit_2[]  = { 0x63, 0x6F, 0x6E, 0x73, 0x74,  antlr3::ANTLR_STRING_TERMINATOR};



/* ============================================================================= */

/* =============================================================================
 * Start of recognizer
 */

using namespace antlr3;


/* =========================================================================
 * Lexer matching rules end.
 * =========================================================================
 */



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
              StringStreamType  err_stream;
                    
              err_stream<< "Lexer error: " <<ex->get_message() << " at offset "
                   << ex->get_charPositionInLine()+1;     
              error_in_lexer = true;    
              
              cerr<<err_stream.str()<<endl;   
          }


OptFileGrammarLexer::~OptFileGrammarLexer()
{
}

void
OptFileGrammarLexer::reset()
{
    this->get_rec()->reset();
}

/** \brief Name of the grammar file that generated this code
 */
static const char fileName[] = "OptFileGrammar.g";

/** \brief Return the name of the grammar file that generated this code.
 */
const char* OptFileGrammarLexer::getGrammarFileName()
{
	return fileName;
}

/** \brief Create a new lexer called OptFileGrammarLexer
 *
 * \param[in]    instream Pointer to an initialized input stream
 * \return
 *     - Success pOptFileGrammarLexer initialized for the lex start
 *     - Fail NULL
 */
OptFileGrammarLexer::OptFileGrammarLexer(StreamType* instream)
:OptFileGrammarLexerImplTraits::BaseLexerType(ANTLR_SIZE_HINT, instream, NULL)
{
	// See if we can create a new lexer with the standard constructor
	//
	this->init(instream );
}

/** \brief Create a new lexer called OptFileGrammarLexer
 *
 * \param[in]    instream Pointer to an initialized input stream
 * \param[state] state Previously created shared recognizer stat
 * \return
 *     - Success pOptFileGrammarLexer initialized for the lex start
 *     - Fail NULL
 */
OptFileGrammarLexer::OptFileGrammarLexer(StreamType* instream, RecognizerSharedStateType* state)
:OptFileGrammarLexerImplTraits::BaseLexerType(ANTLR_SIZE_HINT, instream, state)
{
	this->init(instream );
}

void OptFileGrammarLexer::init(StreamType* instream)
{
    /* -------------------------------------------------------------------
     * Memory for basic structure is allocated, now to fill in
     * in base ANTLR3 structures. We initialize the function pointers
     * for the standard ANTLR3 lexer function set, but upon return
     * from here, the programmer may set the pointers to provide custom
     * implementations of each function.
     *
     * We don't use the macros defined in OptFileGrammarLexer.h here so you can get a sense
     * of what goes where.
     */

}

/* =========================================================================
 * Functions to match the lexer grammar defined tokens from the input stream
 */

//   Comes from: 66:6: ( '+' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__7
 *
 * Looks to match the characters the constitute the token T__7
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mT__7()
{
    ANTLR_UINT32	_type;
      

    _type	    = T__7;


    // OptFileGrammar.g:66:6: ( '+' )
    // OptFileGrammar.g:66:8: '+'
    {
         this->matchc('+');
        if  (this->hasException())
        {
            goto ruleT__7Ex;
        }


    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleT__7Ex; /* Prevent compiler warnings */
    ruleT__7Ex: ;

}
// $ANTLR end T__7

//   Comes from: 67:6: ( '-' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__8
 *
 * Looks to match the characters the constitute the token T__8
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mT__8()
{
    ANTLR_UINT32	_type;
      

    _type	    = T__8;


    // OptFileGrammar.g:67:6: ( '-' )
    // OptFileGrammar.g:67:8: '-'
    {
         this->matchc('-');
        if  (this->hasException())
        {
            goto ruleT__8Ex;
        }


    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleT__8Ex; /* Prevent compiler warnings */
    ruleT__8Ex: ;

}
// $ANTLR end T__8

//   Comes from: 68:6: ( '.' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__9
 *
 * Looks to match the characters the constitute the token T__9
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mT__9()
{
    ANTLR_UINT32	_type;
      

    _type	    = T__9;


    // OptFileGrammar.g:68:6: ( '.' )
    // OptFileGrammar.g:68:8: '.'
    {
         this->matchc('.');
        if  (this->hasException())
        {
            goto ruleT__9Ex;
        }


    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleT__9Ex; /* Prevent compiler warnings */
    ruleT__9Ex: ;

}
// $ANTLR end T__9

//   Comes from: 69:7: ( '//' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__10
 *
 * Looks to match the characters the constitute the token T__10
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mT__10()
{
    ANTLR_UINT32	_type;
      

    _type	    = T__10;


    // OptFileGrammar.g:69:7: ( '//' )
    // OptFileGrammar.g:69:9: '//'
    {
        this->matchs(lit_1);
        if  (this->hasException())
        {
            goto ruleT__10Ex;
        }




    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleT__10Ex; /* Prevent compiler warnings */
    ruleT__10Ex: ;

}
// $ANTLR end T__10

//   Comes from: 70:7: ( ':' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__11
 *
 * Looks to match the characters the constitute the token T__11
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mT__11()
{
    ANTLR_UINT32	_type;
      

    _type	    = T__11;


    // OptFileGrammar.g:70:7: ( ':' )
    // OptFileGrammar.g:70:9: ':'
    {
         this->matchc(':');
        if  (this->hasException())
        {
            goto ruleT__11Ex;
        }


    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleT__11Ex; /* Prevent compiler warnings */
    ruleT__11Ex: ;

}
// $ANTLR end T__11

//   Comes from: 71:7: ( '=' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__12
 *
 * Looks to match the characters the constitute the token T__12
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mT__12()
{
    ANTLR_UINT32	_type;
      

    _type	    = T__12;


    // OptFileGrammar.g:71:7: ( '=' )
    // OptFileGrammar.g:71:9: '='
    {
         this->matchc('=');
        if  (this->hasException())
        {
            goto ruleT__12Ex;
        }


    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleT__12Ex; /* Prevent compiler warnings */
    ruleT__12Ex: ;

}
// $ANTLR end T__12

//   Comes from: 72:7: ( '[' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__13
 *
 * Looks to match the characters the constitute the token T__13
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mT__13()
{
    ANTLR_UINT32	_type;
      

    _type	    = T__13;


    // OptFileGrammar.g:72:7: ( '[' )
    // OptFileGrammar.g:72:9: '['
    {
         this->matchc('[');
        if  (this->hasException())
        {
            goto ruleT__13Ex;
        }


    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleT__13Ex; /* Prevent compiler warnings */
    ruleT__13Ex: ;

}
// $ANTLR end T__13

//   Comes from: 73:7: ( '\\'' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__14
 *
 * Looks to match the characters the constitute the token T__14
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mT__14()
{
    ANTLR_UINT32	_type;
      

    _type	    = T__14;


    // OptFileGrammar.g:73:7: ( '\\'' )
    // OptFileGrammar.g:73:9: '\\''
    {
         this->matchc('\'');
        if  (this->hasException())
        {
            goto ruleT__14Ex;
        }


    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleT__14Ex; /* Prevent compiler warnings */
    ruleT__14Ex: ;

}
// $ANTLR end T__14

//   Comes from: 74:7: ( '\\n' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__15
 *
 * Looks to match the characters the constitute the token T__15
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mT__15()
{
    ANTLR_UINT32	_type;
      

    _type	    = T__15;


    // OptFileGrammar.g:74:7: ( '\\n' )
    // OptFileGrammar.g:74:9: '\\n'
    {
         this->matchc('\n');
        if  (this->hasException())
        {
            goto ruleT__15Ex;
        }


    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleT__15Ex; /* Prevent compiler warnings */
    ruleT__15Ex: ;

}
// $ANTLR end T__15

//   Comes from: 75:7: ( '\\r' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__16
 *
 * Looks to match the characters the constitute the token T__16
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mT__16()
{
    ANTLR_UINT32	_type;
      

    _type	    = T__16;


    // OptFileGrammar.g:75:7: ( '\\r' )
    // OptFileGrammar.g:75:9: '\\r'
    {
         this->matchc('\r');
        if  (this->hasException())
        {
            goto ruleT__16Ex;
        }


    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleT__16Ex; /* Prevent compiler warnings */
    ruleT__16Ex: ;

}
// $ANTLR end T__16

//   Comes from: 76:7: ( ']' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__17
 *
 * Looks to match the characters the constitute the token T__17
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mT__17()
{
    ANTLR_UINT32	_type;
      

    _type	    = T__17;


    // OptFileGrammar.g:76:7: ( ']' )
    // OptFileGrammar.g:76:9: ']'
    {
         this->matchc(']');
        if  (this->hasException())
        {
            goto ruleT__17Ex;
        }


    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleT__17Ex; /* Prevent compiler warnings */
    ruleT__17Ex: ;

}
// $ANTLR end T__17

//   Comes from: 77:7: ( 'const' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__18
 *
 * Looks to match the characters the constitute the token T__18
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mT__18()
{
    ANTLR_UINT32	_type;
      

    _type	    = T__18;


    // OptFileGrammar.g:77:7: ( 'const' )
    // OptFileGrammar.g:77:9: 'const'
    {
        this->matchs(lit_2);
        if  (this->hasException())
        {
            goto ruleT__18Ex;
        }




    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleT__18Ex; /* Prevent compiler warnings */
    ruleT__18Ex: ;

}
// $ANTLR end T__18

//   Comes from: 160:3: ( ( '0' .. '9' )+ ( '.' ( '0' .. '9' )+ )? )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start Number
 *
 * Looks to match the characters the constitute the token Number
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mNumber()
{
    ANTLR_UINT32	_type;
      

    _type	    = Number;


    // OptFileGrammar.g:160:3: ( ( '0' .. '9' )+ ( '.' ( '0' .. '9' )+ )? )
    // OptFileGrammar.g:161:3: ( '0' .. '9' )+ ( '.' ( '0' .. '9' )+ )?
    {
        // OptFileGrammar.g:161:3: ( '0' .. '9' )+
        {
            int cnt1=0;

            for (;;)
            {
                int alt1=2;
        	switch ( this->LA(1) )
        	{
        	case '0':
        	case '1':
        	case '2':
        	case '3':
        	case '4':
        	case '5':
        	case '6':
        	case '7':
        	case '8':
        	case '9':
        		{
        			alt1=1;
        		}
        	    break;

        	}

        	switch (alt1)
        	{
        	    case 1:
        	        // OptFileGrammar.g:
        	        {
        	            if ( ((this->LA(1) >= '0') && (this->LA(1) <= '9')) )
        	            {
        	                this->consume();
        	            }
        	            else
        	            {
        	                new ANTLR_Exception< OptFileGrammarLexerImplTraits, MISMATCHED_SET_EXCEPTION, StreamType>( this->get_rec(), "" );

        	                this->recover();
        	                goto ruleNumberEx;
        	            }


        	        }
        	        break;

        	    default:

        		if ( cnt1 >= 1 )
        		{
        		    goto loop1;
        		}
        		/* mismatchedSetEx()
        		 */
        		new ANTLR_Exception< OptFileGrammarLexerImplTraits, EARLY_EXIT_EXCEPTION, StreamType>( this->get_rec(), "" );


        		goto ruleNumberEx;
        	}
        	cnt1++;
            }
            loop1: ;	/* Jump to here if this rule does not match */
        }

        // OptFileGrammar.g:161:15: ( '.' ( '0' .. '9' )+ )?
        {
            int alt3=2;
            switch ( this->LA(1) )
            {
                case '.':
                	{
                		alt3=1;
                	}
                    break;
            }

            switch (alt3)
            {
        	case 1:
        	    // OptFileGrammar.g:161:16: '.' ( '0' .. '9' )+
        	    {
        	         this->matchc('.');
        	        if  (this->hasException())
        	        {
        	            goto ruleNumberEx;
        	        }


        	        // OptFileGrammar.g:161:20: ( '0' .. '9' )+
        	        {
        	            int cnt2=0;

        	            for (;;)
        	            {
        	                int alt2=2;
        	        	switch ( this->LA(1) )
        	        	{
        	        	case '0':
        	        	case '1':
        	        	case '2':
        	        	case '3':
        	        	case '4':
        	        	case '5':
        	        	case '6':
        	        	case '7':
        	        	case '8':
        	        	case '9':
        	        		{
        	        			alt2=1;
        	        		}
        	        	    break;

        	        	}

        	        	switch (alt2)
        	        	{
        	        	    case 1:
        	        	        // OptFileGrammar.g:
        	        	        {
        	        	            if ( ((this->LA(1) >= '0') && (this->LA(1) <= '9')) )
        	        	            {
        	        	                this->consume();
        	        	            }
        	        	            else
        	        	            {
        	        	                new ANTLR_Exception< OptFileGrammarLexerImplTraits, MISMATCHED_SET_EXCEPTION, StreamType>( this->get_rec(), "" );

        	        	                this->recover();
        	        	                goto ruleNumberEx;
        	        	            }


        	        	        }
        	        	        break;

        	        	    default:

        	        		if ( cnt2 >= 1 )
        	        		{
        	        		    goto loop2;
        	        		}
        	        		/* mismatchedSetEx()
        	        		 */
        	        		new ANTLR_Exception< OptFileGrammarLexerImplTraits, EARLY_EXIT_EXCEPTION, StreamType>( this->get_rec(), "" );


        	        		goto ruleNumberEx;
        	        	}
        	        	cnt2++;
        	            }
        	            loop2: ;	/* Jump to here if this rule does not match */
        	        }

        	    }
        	    break;

            }
        }

    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleNumberEx; /* Prevent compiler warnings */
    ruleNumberEx: ;

}
// $ANTLR end Number

//   Comes from: 171:3: ( ( ' ' | '\\t' ) )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start WS
 *
 * Looks to match the characters the constitute the token WS
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mWS()
{
    ANTLR_UINT32	_type;
      

    _type	    = WS;


    // OptFileGrammar.g:171:3: ( ( ' ' | '\\t' ) )
    // OptFileGrammar.g:172:3: ( ' ' | '\\t' )
    {
        if ( this->LA(1) == '\t' || this->LA(1) == ' ' )
        {
            this->consume();
        }
        else
        {
            new ANTLR_Exception< OptFileGrammarLexerImplTraits, MISMATCHED_SET_EXCEPTION, StreamType>( this->get_rec(), "" );

            this->recover();
            goto ruleWSEx;
        }


        {

               //this->get_state()->get_channel()=HIDDEN; //we need white spaces to differentiate sperate const keyword from the variable name
              
        }


    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleWSEx; /* Prevent compiler warnings */
    ruleWSEx: ;

}
// $ANTLR end WS

//   Comes from: 183:3: ( ( 'a' .. 'z' | 'A' .. 'Z' | '_' ) ( 'a' .. 'z' | 'A' .. 'Z' | '_' | '0' .. '9' )* )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start Identifier
 *
 * Looks to match the characters the constitute the token Identifier
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == true if an exception was thrown.
 */
void OptFileGrammarLexer::mIdentifier()
{
    ANTLR_UINT32	_type;
      

    _type	    = Identifier;


    // OptFileGrammar.g:183:3: ( ( 'a' .. 'z' | 'A' .. 'Z' | '_' ) ( 'a' .. 'z' | 'A' .. 'Z' | '_' | '0' .. '9' )* )
    // OptFileGrammar.g:184:3: ( 'a' .. 'z' | 'A' .. 'Z' | '_' ) ( 'a' .. 'z' | 'A' .. 'Z' | '_' | '0' .. '9' )*
    {
        if ( ((this->LA(1) >= 'A') && (this->LA(1) <= 'Z')) || this->LA(1) == '_' || ((this->LA(1) >= 'a') && (this->LA(1) <= 'z')) )
        {
            this->consume();
        }
        else
        {
            new ANTLR_Exception< OptFileGrammarLexerImplTraits, MISMATCHED_SET_EXCEPTION, StreamType>( this->get_rec(), "" );

            this->recover();
            goto ruleIdentifierEx;
        }


        // OptFileGrammar.g:189:3: ( 'a' .. 'z' | 'A' .. 'Z' | '_' | '0' .. '9' )*

        for (;;)
        {
            int alt4=2;
            switch ( this->LA(1) )
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            	{
            		alt4=1;
            	}
                break;

            }

            switch (alt4)
            {
        	case 1:
        	    // OptFileGrammar.g:
        	    {
        	        if ( ((this->LA(1) >= '0') && (this->LA(1) <= '9')) || ((this->LA(1) >= 'A') && (this->LA(1) <= 'Z')) || this->LA(1) == '_' || ((this->LA(1) >= 'a') && (this->LA(1) <= 'z')) )
        	        {
        	            this->consume();
        	        }
        	        else
        	        {
        	            new ANTLR_Exception< OptFileGrammarLexerImplTraits, MISMATCHED_SET_EXCEPTION, StreamType>( this->get_rec(), "" );

        	            this->recover();
        	            goto ruleIdentifierEx;
        	        }


        	    }
        	    break;

        	default:
        	    goto loop4;	/* break out of the loop */
        	    break;
            }
        }
        loop4: ; /* Jump out to here if this rule does not match */


    }

	this->get_lexstate()->set_type(_type);
    // This is where rules clean up and exit
    //
    goto ruleIdentifierEx; /* Prevent compiler warnings */
    ruleIdentifierEx: ;

}
// $ANTLR end Identifier

/** This is the entry point in to the lexer from an object that
 *  wants to generate the next token, such as a pCOMMON_TOKEN_STREAM
 */
void
OptFileGrammarLexer::mTokens()
{
    {
        //  OptFileGrammar.g:1:8: ( T__7 | T__8 | T__9 | T__10 | T__11 | T__12 | T__13 | T__14 | T__15 | T__16 | T__17 | T__18 | Number | WS | Identifier )

        ANTLR_UINT32 alt5;

        alt5=15;

        switch ( this->LA(1) )
        {
        case '+':
        	{
        		alt5=1;
        	}
            break;
        case '-':
        	{
        		alt5=2;
        	}
            break;
        case '.':
        	{
        		alt5=3;
        	}
            break;
        case '/':
        	{
        		alt5=4;
        	}
            break;
        case ':':
        	{
        		alt5=5;
        	}
            break;
        case '=':
        	{
        		alt5=6;
        	}
            break;
        case '[':
        	{
        		alt5=7;
        	}
            break;
        case '\'':
        	{
        		alt5=8;
        	}
            break;
        case '\n':
        	{
        		alt5=9;
        	}
            break;
        case '\r':
        	{
        		alt5=10;
        	}
            break;
        case ']':
        	{
        		alt5=11;
        	}
            break;
        case 'c':
        	{
        		switch ( this->LA(2) )
        		{
        		case 'o':
        			{
        				switch ( this->LA(3) )
        				{
        				case 'n':
        					{
        						switch ( this->LA(4) )
        						{
        						case 's':
        							{
        								switch ( this->LA(5) )
        								{
        								case 't':
        									{
        										switch ( this->LA(6) )
        										{
        										case '0':
        										case '1':
        										case '2':
        										case '3':
        										case '4':
        										case '5':
        										case '6':
        										case '7':
        										case '8':
        										case '9':
        										case 'A':
        										case 'B':
        										case 'C':
        										case 'D':
        										case 'E':
        										case 'F':
        										case 'G':
        										case 'H':
        										case 'I':
        										case 'J':
        										case 'K':
        										case 'L':
        										case 'M':
        										case 'N':
        										case 'O':
        										case 'P':
        										case 'Q':
        										case 'R':
        										case 'S':
        										case 'T':
        										case 'U':
        										case 'V':
        										case 'W':
        										case 'X':
        										case 'Y':
        										case 'Z':
        										case '_':
        										case 'a':
        										case 'b':
        										case 'c':
        										case 'd':
        										case 'e':
        										case 'f':
        										case 'g':
        										case 'h':
        										case 'i':
        										case 'j':
        										case 'k':
        										case 'l':
        										case 'm':
        										case 'n':
        										case 'o':
        										case 'p':
        										case 'q':
        										case 'r':
        										case 's':
        										case 't':
        										case 'u':
        										case 'v':
        										case 'w':
        										case 'x':
        										case 'y':
        										case 'z':
        											{
        												alt5=15;
        											}
        										    break;

        										default:
        										    alt5=12;
        										}

        									}
        								    break;

        								default:
        								    alt5=15;
        								}

        							}
        						    break;

        						default:
        						    alt5=15;
        						}

        					}
        				    break;

        				default:
        				    alt5=15;
        				}

        			}
        		    break;

        		default:
        		    alt5=15;
        		}

        	}
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        	{
        		alt5=13;
        	}
            break;
        case '\t':
        case ' ':
        	{
        		alt5=14;
        	}
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case '_':
        case 'a':
        case 'b':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
        	{
        		alt5=15;
        	}
            break;

        default:
            ExceptionBaseType* ex = new ANTLR_Exception< OptFileGrammarLexerImplTraits, NO_VIABLE_ALT_EXCEPTION, StreamType>( this->get_rec(), "" );
            ex->set_decisionNum( 5 );
            ex->set_state( 0 );


            goto ruleTokensEx;

        }

        switch (alt5)
        {
    	case 1:
    	    // OptFileGrammar.g:1:10: T__7
    	    {
    	        /* 1:10: T__7 */
    	        mT__7();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 2:
    	    // OptFileGrammar.g:1:15: T__8
    	    {
    	        /* 1:15: T__8 */
    	        mT__8();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 3:
    	    // OptFileGrammar.g:1:20: T__9
    	    {
    	        /* 1:20: T__9 */
    	        mT__9();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 4:
    	    // OptFileGrammar.g:1:25: T__10
    	    {
    	        /* 1:25: T__10 */
    	        mT__10();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 5:
    	    // OptFileGrammar.g:1:31: T__11
    	    {
    	        /* 1:31: T__11 */
    	        mT__11();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 6:
    	    // OptFileGrammar.g:1:37: T__12
    	    {
    	        /* 1:37: T__12 */
    	        mT__12();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 7:
    	    // OptFileGrammar.g:1:43: T__13
    	    {
    	        /* 1:43: T__13 */
    	        mT__13();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 8:
    	    // OptFileGrammar.g:1:49: T__14
    	    {
    	        /* 1:49: T__14 */
    	        mT__14();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 9:
    	    // OptFileGrammar.g:1:55: T__15
    	    {
    	        /* 1:55: T__15 */
    	        mT__15();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 10:
    	    // OptFileGrammar.g:1:61: T__16
    	    {
    	        /* 1:61: T__16 */
    	        mT__16();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 11:
    	    // OptFileGrammar.g:1:67: T__17
    	    {
    	        /* 1:67: T__17 */
    	        mT__17();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 12:
    	    // OptFileGrammar.g:1:73: T__18
    	    {
    	        /* 1:73: T__18 */
    	        mT__18();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 13:
    	    // OptFileGrammar.g:1:79: Number
    	    {
    	        /* 1:79: Number */
    	        mNumber();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 14:
    	    // OptFileGrammar.g:1:86: WS
    	    {
    	        /* 1:86: WS */
    	        mWS();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;
    	case 15:
    	    // OptFileGrammar.g:1:89: Identifier
    	    {
    	        /* 1:89: Identifier */
    	        mIdentifier();
    	        if  (this->hasException())
    	        {
    	            goto ruleTokensEx;
    	        }



    	    }
    	    break;

        }
    }


    goto ruleTokensEx; /* Prevent compiler warnings */
ruleTokensEx: ;
}

/* =========================================================================
 * Lexer matching rules end.
 * =========================================================================
 */
/* End of Lexer code
 * ================================================
 * ================================================
 */



/* End of code
 * =============================================================================
 */
