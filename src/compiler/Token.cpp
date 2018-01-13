// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <compiler/Token.hpp>


namespace jupiter{

    Token::Token( TokenType type, std::string& value ){
        this->type = type;
        this->value = value;
    }

    Token::Token( TokenType type ){
        this->type = type;
    }

    TokenType Token::getType(){
        return this->type;
    }

    std::string& Token::getValue(){
        return this->value;
    }

    bool Token::is( TokenType type ){
        return type == this->type;
    }

    std::string Token::toString(){

        switch ( this->type ){

        case DOT:               return ".";
        case COMMA:             return ",";
        case SEMICOLON:         return ";";
        case L_PAREN:           return "(";
        case R_PAREN:           return ")";
        case L_BRACKET:         return "{";
        case R_BRACKET:         return "}";
        case L_SQPAREN:         return "[";
        case R_SQPAREN:         return "]";
        case ASSIGNMENT:        return ":=";
        case OPERATOR:          return value + " - OPERATOR";
        case SYMBOL:            return value + " - SYMBOL";
        case MESSAGE_KEYWORD:   return value + " - MESSAGE KEYWORD" ;
        case KEYWORD:           return value + " - KEYWORD";
        case NUMBER:            return value + " - NUMBER";
        case STRING:            return value + " - STRING";
        default:                return "Token without to string repr";

        }

    }
}
