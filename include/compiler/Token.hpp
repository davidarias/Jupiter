// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __TOKEN_H
#define __TOKEN_H

#include <misc/common.hpp>

namespace jupiter{

    enum TokenType{
        L_PAREN,
        R_PAREN,
        L_BRACKET,
        R_BRACKET,
        L_SQPAREN,
        R_SQPAREN,
        ASSIGNMENT,
        DOT,
        SEMICOLON,
        COMMA,
        OPERATOR,
        SYMBOL,
        MESSAGE_KEYWORD,
        KEYWORD,
        NUMBER,
        STRING
    };

    class Token{
        private:
            TokenType type;
            std::string value;

        public:
            Token( TokenType type, std::string& value );
            Token( TokenType type );

            std::string toString();

            TokenType getType();
            std::string& getValue();
            bool is( TokenType type );

    };

}
#endif
