// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __LEXER_H
#define __LEXER_H

#include <compiler/Token.hpp>

namespace jupiter{

    class Lexer{

    private:
        std::string source;
        std::vector<Token> tokenList;

        std::string::iterator sourceIterator;
        std::string::iterator sourceEnd;

        int parensBalance;
        int sqParensBalance;
        int unsigned bracketsBalance;

        bool isEnd();

        void advance();
        void skipWhiteSpaces();
        void skipComment();

        char peek();

        void scan();
        void scanNumber();
        void scanSymbol();
        void scanKeyword();
        void scanString();
        void scanOperator();

    public:
        Lexer(std::string source);

        // return shared_pointer
        std::vector<Token>& tokenize();

    };
}
#endif
