// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <compiler/Lexer.hpp>

#include <misc/Exceptions.hpp>

namespace jupiter{

    bool isDoubleQuote( char character ){
        return character == '\"';
    }

    bool isQuote( char character ){
        return character == '\'';
    }

    // + / \ * ~ < > = @ % | & ? !
    bool isOperator( char character ){
        switch (character){
        case '+':
        case '-':
        case '*':
        case '/':
        case '\\':
        case '~':
        case '>':
        case '<':
        case '=':
        case '@':
        case '#':
        case '%':
        case '|':
        case '&':
        case '?':
        case '!':
            return true;
        default:
            return false;
        }
    }

    bool isValidCharForSymbol( char character ){
        if ( std::isalpha( character ) || std::isdigit( character ) ){
            return true;
        }
        switch (character){
        case '$':
            return true;
        default:
            return false;
        }
    }

    Lexer::Lexer(std::string source)
        : source(source), parensBalance(0), sqParensBalance(0), bracketsBalance(0) {}

    bool Lexer::isEnd(){
        return this->sourceIterator == this->sourceEnd;
    }

    void Lexer::skipWhiteSpaces(){

        while( ! this->isEnd() && isspace( *sourceIterator ) ){

            ++sourceIterator;

        }
    }

    void Lexer::skipComment(){

        // iterator is at the opening double quote
        advance();

        while( ! isEnd() && ! isDoubleQuote( *sourceIterator ) ){

            ++sourceIterator;
        }

    }

    void Lexer::advance(){
        if ( ! isEnd() ) sourceIterator++;
    }

    char Lexer::peek(){
        if ( ! isEnd() ){
            auto nextIterator = std::next( sourceIterator );
            // don't deref iterator end, bad things happen if you do so.
            if ( nextIterator != sourceEnd )
                return *nextIterator;
        }
        return '\0';
    }

    void Lexer::scanNumber(){
        std::string numberValue;

        // should be a digit or a '-' sign (minus)
        numberValue.push_back( *sourceIterator );
        ++sourceIterator;

        while( ! this->isEnd() && ( std::isdigit( *sourceIterator ) || *sourceIterator == '.' ) ){
            numberValue.push_back( *sourceIterator );
            ++sourceIterator;
        }

        char back = numberValue.back();

        // if last char is '.' we have to emit the number and DOT token,
        // since a number cannot end with the '.' char
        if ( back == '.' ){
            numberValue.pop_back();
            this->tokenList.push_back( Token( NUMBER, numberValue ) );
            this->tokenList.push_back( Token( DOT ) );

        }else{
            this->tokenList.push_back( Token( NUMBER, numberValue ) );
        }


    }

    void Lexer::scanKeyword(){

        std::string symbolValue;

        advance(); // skip the ':' (colon) char

        while( ! this->isEnd() && isValidCharForSymbol( *sourceIterator ) ){
            symbolValue.push_back( *sourceIterator );
            ++sourceIterator;
        }

        tokenList.push_back( Token( KEYWORD, symbolValue ) );

    }

    void Lexer::scanSymbol(){

        std::string symbolValue;

        while( ! this->isEnd() && isValidCharForSymbol( *sourceIterator ) ){
            symbolValue.push_back( *sourceIterator );
            ++sourceIterator;
        }

        // can be a message keyword or an asignment expresion
        if ( *sourceIterator == ':' ){

            if ( peek() == '=' ){
                // is an asignment

                tokenList.push_back( Token( SYMBOL, symbolValue ) );
                tokenList.push_back( Token( ASSIGNMENT ) );

                advance();advance(); // leave iterator at next char ( as expected in scan method )
            }else{
                // message keyword

                symbolValue.push_back(':'); // the ':' char is part of the message
                this->tokenList.push_back( Token( MESSAGE_KEYWORD, symbolValue ) );

                 // skip the colon char and leave iterator at next char ( as expected in scan method )
                advance();
            }

        }else{

            this->tokenList.push_back( Token( SYMBOL, symbolValue ) );
        }

    }

    void Lexer::scanString(){
        std::string value;

        // iterator is at the opening double quote
        advance();

        while( ! this->isEnd() && ! isQuote( *sourceIterator ) ){

            value.push_back( *sourceIterator );
            ++sourceIterator;
        }

        this->tokenList.push_back( Token( STRING, value ) );

    }

    void Lexer::scanOperator(){
        std::string value;


        while( ! this->isEnd() && isOperator( *sourceIterator ) ){

            value.push_back( *sourceIterator );
            ++sourceIterator;
        }

        this->tokenList.push_back( Token( OPERATOR, value ) );

    }

    void Lexer::scan(){

        this->skipWhiteSpaces();

        if ( isDoubleQuote( *sourceIterator ) ){

            skipComment();

            // iterator is at the closing quote
            advance();

        } else if ( std::isdigit( *sourceIterator ) ||
                    ( *sourceIterator == '-' && std::isdigit( peek() ) ) ){

            this->scanNumber();

            // iterator is already at next char, no need for advance

        } else if ( *sourceIterator == ':'){

            if ( peek() == '='){

                this->tokenList.push_back( Token( ASSIGNMENT ) );
                advance(); advance(); // skip the '=' char

            }else{

                scanKeyword();
                // iterator is already at next char, no need for advance
            }

        }  else if ( isValidCharForSymbol( *sourceIterator ) ){

            this->scanSymbol();

            // iterator is already at next char, no need for advance

        } else if ( isOperator( *sourceIterator ) ) {

            this->scanOperator();
            // iterator is already at next char, no need for advance

        } else if ( isQuote( *sourceIterator ) ) {

            this->scanString();

            // iterator is at the closing quote
            advance();

        } else if ( *sourceIterator == ';' ){

            this->tokenList.push_back( Token( SEMICOLON ) );

            advance();

        }  else if ( *sourceIterator == '.' ){

            this->tokenList.push_back( Token( DOT ) );

            advance();

        } else if ( *sourceIterator == ',' ){

            this->tokenList.push_back( Token( COMMA ) );

            advance();

        } else if ( *sourceIterator == '('){

            parensBalance++;

            this->tokenList.push_back( Token( L_PAREN ) );
            advance();

        } else if ( *sourceIterator == ')' ){

            parensBalance--;

            this->tokenList.push_back( Token( R_PAREN ) );
            advance();

        } else if ( *sourceIterator == '[' ){

            sqParensBalance++;

            this->tokenList.push_back( Token( L_SQPAREN ) );
            advance();

        } else if ( *sourceIterator == ']' ){

            sqParensBalance--;

            this->tokenList.push_back( Token( R_SQPAREN ) );
            advance();

        } else if ( *sourceIterator == '{' ){

            bracketsBalance++;

            this->tokenList.push_back( Token( L_BRACKET ) );
            advance();

        } else if ( *sourceIterator == '}' ){

            bracketsBalance--;

            this->tokenList.push_back( Token( R_BRACKET ) );
            advance();

        }else{

            // ignore current char
            advance();

        }

    }

    void printTokens(std::vector<Token>& tokens){
        LOG( "" );
        LOG( "-----------------" );
        LOG( "-- DEBUG LEXER --" );
        LOG( "-----------------" );
        LOG( "" );

        auto iterator = tokens.begin();
        for (; iterator != tokens.end(); iterator++) {

            LOG( iterator->toString() );

        }
        LOG( "" );
        LOG( "---------------------" );
        LOG( "-- END DEBUG LEXER --" );
        LOG( "---------------------" );
        LOG( "" );
    }

    std::vector<Token>& Lexer::tokenize(){

        sourceIterator = source.begin();
        sourceEnd = source.end();

        while( ! this->isEnd() ) this->scan();

        if ( parensBalance != 0) throw CompilerError("unbalanced parentheses");
        if ( sqParensBalance != 0) throw CompilerError("unbalanced squared parentheses");
        if ( bracketsBalance != 0) throw CompilerError("unbalanced brackets");

        // printTokens( tokenList );

        return this->tokenList;

    }


}
