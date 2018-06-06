// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __PARSER_H
#define __PARSER_H

#include <misc/common.hpp>
#include <compiler/Token.hpp>
#include <compiler/ASTNode.hpp>

namespace jupiter{

    class Parser{

    private:

        std::vector<Token>::iterator tokensIterator;
        std::vector<Token>::iterator tokensEnd;

        bool isEnd();
        bool isLastToken();
        Token* getNextToken();
        Token* getNextTokenOrThrow();
        Token* peek();
        Token* getCurrentToken();
        void advance();

        std::shared_ptr<ASTNode> parseNumber();
        std::shared_ptr<SymbolNode> parseSymbol();
        std::shared_ptr<ASTNode> parseString();
        std::shared_ptr<ASTNode> parseArray();
        std::shared_ptr<ASTNode> parseClosureBlock();
        std::shared_ptr<ASTNode> parseParentheses();
        std::shared_ptr<ASTNode> parseObject();

        std::shared_ptr<ASTNode> parseExpression();
        std::shared_ptr<ASTNode> parseAssignment();
        std::shared_ptr<ASTNode> parseAssignmentRvalue();
        std::shared_ptr<ASTNode> parseMessageExpression();
        std::shared_ptr<ASTNode> parseUnaryMessage();
        std::shared_ptr<MessageNode> parseUnaryMessage(std::shared_ptr<ASTNode> receiver);
        std::shared_ptr<ASTNode> parseBinaryMessage();
        std::shared_ptr<MessageNode> parseBinaryMessage(std::shared_ptr<ASTNode> receiver);
        std::shared_ptr<ASTNode> parseKeywordMessage();
        std::shared_ptr<MessageNode> parseKeywordMessage(std::shared_ptr<ASTNode> receiver);
        std::shared_ptr<ASTNode> parseCascadeMessage();

        std::shared_ptr<ASTNode> parsePragma();

        std::shared_ptr<ASTNode> parseCodeBlock();

    public:
        Parser(std::vector<Token>& tokens);

        std::shared_ptr<ASTNode> parse();
        std::shared_ptr<MethodSignature> parseMethodSignature();
    private:
        std::shared_ptr<SymbolNode> parseArgumentName();

    };
}
#endif
