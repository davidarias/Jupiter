// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <misc/common.hpp>

#include <compiler/Parser.hpp>
#include <misc/Exceptions.hpp>

namespace jupiter{

    Parser::Parser(std::vector<Token>& tokens) {
        tokensIterator = tokens.begin();
        tokensEnd = tokens.end();
    }

    std::shared_ptr<ASTNode> Parser::parseNumber(){
        auto token = getCurrentToken();
        std::string& tokenValue = token->getValue();
        auto astNode = std::make_shared<NumberNode>( tokenValue );
        return astNode;
    }

    std::shared_ptr<SymbolNode> Parser::parseSymbol(){
        auto token = getCurrentToken();
        std::string& tokenValue = token->getValue();
        // TODO use interned strings
        auto astNode = std::make_shared<SymbolNode>( tokenValue );
        return astNode;
    }

    std::shared_ptr<ASTNode> Parser::parseString(){
        auto token = getCurrentToken();
        std::string& tokenValue = token->getValue();
        // TODO use interned strings
        auto astNode = std::make_shared<StringNode>( tokenValue );
        return astNode;
    }

    std::shared_ptr<ASTNode> Parser::parseArray(){
        advance(); //skip the '{' token

        if ( ! isEnd() && getCurrentToken()->is( MESSAGE_KEYWORD ) ){
            throw CompilerError("Unexpected key in array. Maybe you want to define a Map using literal syntax #{ ... }");
        }

        auto node =  std::make_shared<ArrayNode>();

        while ( ! isEnd() && ! getCurrentToken()->is( R_BRACKET ) ){
            node->values.push_back( parseCascadeMessage() );

            if ( ! isEnd() && getCurrentToken()->is( COMMA ) ){
                advance();
            }

        }
        return node;
    }

    std::shared_ptr<ASTNode> Parser::parseClosureBlock(){

        advance(); //skip the '[' token


        auto node = std::make_shared<ClosureBlockNode>();
        auto codeBlockNode = std::make_shared<CodeBlockNode>();

        // check closure block arguments
        if ( getCurrentToken()->is(KEYWORD) ){


            while( ! isEnd() && getCurrentToken()->getValue() != "|" ){

                if ( getCurrentToken()->is(KEYWORD)){

                    node->arguments.push_back( parseSymbol() );

                }else{
                    throw SyntaxError("Unexcepted token in closureblock arguments list: '"
                                      + getCurrentToken()->toString() + "'");
                }
                advance();
            }

            // skip the | token
            advance();

        }

        // parse code
        while ( ! isEnd() && ! getCurrentToken()->is(R_SQPAREN) ){

            // parse expresion should leave currentToken to the next expresion, DOT or ]
            auto node = parseExpression();
            codeBlockNode->nodes.push_back( node );

            if ( getCurrentToken()->is( DOT ) ){
                // ignore DOT
                advance();
            }

        }

        node->code = codeBlockNode;

        // advance(); // skip the ']' token

        return node;

    }

    std::shared_ptr<ASTNode> Parser::parseParentheses(){
        advance(); //skip the '(' token
        return parseCascadeMessage();
    }

    std::shared_ptr<ASTNode> Parser::parseObject(){
        auto token = getCurrentToken();
        switch( token->getType() ){
        case NUMBER:
            return parseNumber();
        case SYMBOL:
            return parseSymbol();
         case STRING:
            return parseString();
        case L_SQPAREN:
            return parseClosureBlock();
        case L_PAREN:
            return parseParentheses();
        case L_BRACKET:
            return parseArray();
        default:
            throw CompilerError("parseObject: Unexpected token: " + token->toString() );
        }
    }

    std::shared_ptr<ASTNode> Parser::parseCascadeMessage(){
        bool isCascade = false;
        auto node = parseKeywordMessage();

        auto message = std::dynamic_pointer_cast<MessageNode>(node);
        auto currentToken = getCurrentToken();

        if (! message ){
            if ( currentToken && currentToken->is( SEMICOLON ) ){
                throw CompilerError("Unexpected cascade on non-message node");
            }else{
                return node;
            }
        }

        std::vector<std::shared_ptr<MessageNode>> messages;
        messages.push_back( message );

        while( currentToken && currentToken->is( SEMICOLON ) ){
            isCascade = true;

            currentToken = getNextToken();

            if ( currentToken == nullptr ) throw CompilerError("Unexpected end of input at message cascade");

            switch( currentToken->getType()){

            case SYMBOL:
                messages.push_back( parseUnaryMessage( nullptr ) );
                advance(); // parse unary leaves token at current
                break;
            case OPERATOR:
                messages.push_back( parseBinaryMessage( nullptr ) );
                break;
            case MESSAGE_KEYWORD:
                messages.push_back( parseKeywordMessage( nullptr ) );
                break;
            default:
                throw CompilerError("Unexpected token at message cascade");
                break;
            }
            currentToken = getCurrentToken();
        }

        if ( isCascade ){
            // XXX Note that only the first message has a valid receiver!
            auto cascadeNode = std::make_shared<CascadeMessageNode>();
            cascadeNode->messages = messages;
            return cascadeNode;
        }

        return node;
    }

    std::shared_ptr<MessageNode> Parser::parseUnaryMessage( std::shared_ptr<ASTNode> receiver ){
        auto node = std::make_shared<MessageNode>();
        auto token = getCurrentToken();
        node->receiver = receiver;
        node->selector = token->getValue();
        return node;
    }

    std::shared_ptr<ASTNode> Parser::parseUnaryMessage(){
        std::shared_ptr<ASTNode> node = parseObject();

        auto currentToken = getNextToken();

        while( currentToken && currentToken->is( SYMBOL ) ){

            node = parseUnaryMessage( node );
            currentToken = getNextToken();

        }

        return node;

    }

    std::shared_ptr<MessageNode> Parser::parseBinaryMessage(std::shared_ptr<ASTNode> receiver){
        auto node = std::make_shared<MessageNode>();
        auto token = getCurrentToken();
        node->receiver = receiver;
        node->selector = token->getValue();
        advance();
        node->arguments.push_back( parseUnaryMessage() );
        return node;
    }

    std::shared_ptr<ASTNode> Parser::parseBinaryMessage(){
        std::shared_ptr<ASTNode> node = parseUnaryMessage();

        auto currentToken = getCurrentToken();

        while( currentToken && currentToken->is( OPERATOR ) ){

            node = parseBinaryMessage( node );
            currentToken = getCurrentToken();

        }

        return node;
    }

    std::shared_ptr<MessageNode> Parser::parseKeywordMessage(std::shared_ptr<ASTNode> receiver){
        auto node = std::make_shared<MessageNode>();
        auto currentToken = getCurrentToken();
        while( currentToken && currentToken->is( MESSAGE_KEYWORD ) ){
            node->receiver = receiver;
            node->selector += currentToken->getValue();
            advance();
            node->arguments.push_back( parseBinaryMessage() );
            currentToken = getCurrentToken();
        }
        return node;
    }

    std::shared_ptr<ASTNode> Parser::parseKeywordMessage(){
        auto receiver = parseBinaryMessage();
        auto currentToken = getCurrentToken();

        if ( currentToken == nullptr ) return receiver;
        if ( ! currentToken->is( MESSAGE_KEYWORD ) ) return receiver;

        return parseKeywordMessage( receiver );
    }

    std::shared_ptr<ASTNode> Parser::parseAssignment(){
        auto node = std::make_shared<AssignmentNode>();

        node->symbol = parseSymbol();
        advance(); advance(); // skip the ':=' assignment token
        node->value = parseAssignmentRvalue();

        return node;

    }

    std::shared_ptr<ASTNode> Parser::parsePragma(){
        auto currentToken = getNextToken();

        if ( currentToken == nullptr )
            throw CompilerError("parsePragma: unexpected end of input");
        if ( ! currentToken->is( MESSAGE_KEYWORD ) )
            throw CompilerError("parsePragma: unexpected token" + currentToken->toString());

        auto node = std::make_shared<PragmaNode>();

        while( currentToken && currentToken->is( MESSAGE_KEYWORD ) ){

            node->selector += currentToken->getValue();
            advance();
            // TODO allow only strings or integers
            node->arguments.push_back( parseObject() );

            currentToken = getNextToken();
        }

        if ( currentToken->is(OPERATOR) && currentToken->getValue() == ">" ){
            advance();
        }else{
            throw CompilerError("Unexpected end of pragma");
        }

        return node;

    }

    std::shared_ptr<ASTNode> Parser::parseAssignmentRvalue(){
        auto currentToken = getCurrentToken();
        auto nextToken = peek();

        if ( currentToken->is(SYMBOL) && nextToken && nextToken->is(ASSIGNMENT) ){
            return parseAssignment();
        }else{
            return parseCascadeMessage();
        }

    }

    std::shared_ptr<ASTNode> Parser::parseMessageExpression(){
        auto node = std::make_shared<MessageExpressionNode>();
        node->message = parseCascadeMessage();
        return node;
    }

    std::shared_ptr<ASTNode> Parser::parseExpression(){
        auto currentToken = getCurrentToken();
        auto nextToken = peek();

        if ( currentToken->is(OPERATOR) && currentToken->getValue() == "<" ){
            return parsePragma();
        }else if ( currentToken->is(SYMBOL) && nextToken && nextToken->is(ASSIGNMENT) ){
            return parseAssignment();
        }else{
            return parseMessageExpression();
        }

    }

    std::shared_ptr<ASTNode> Parser::parseCodeBlock(){

        auto codeBlockNode = std::make_shared<CodeBlockNode>();

        while ( ! isEnd() ){

            auto node = parseExpression();
            codeBlockNode->nodes.push_back( node );

            advance();

        }

        return codeBlockNode;
    }

    std::shared_ptr<SymbolNode> Parser::parseArgumentName(){
        auto token = getCurrentToken();
        std::string& tokenValue = token->getValue();
        // TODO use interned strings
        auto astNode = std::make_shared<SymbolNode>( tokenValue );
        return astNode;
    }

    std::shared_ptr<MethodSignature> Parser::parseMethodSignature(){
        // TODO refactor this ( maybe in a different class ? )
        auto currentToken = getCurrentToken();

        if ( currentToken == nullptr )
            throw CompilerError("parsePragma: unexpected end of input");

        auto node = std::make_shared<MethodSignature>();

        switch( currentToken->getType() ){

        case MESSAGE_KEYWORD:

            while( currentToken && currentToken->is( MESSAGE_KEYWORD ) ){

                node->selector += currentToken->getValue();
                advance();

                node->arguments.push_back( parseArgumentName() );

                currentToken = getNextToken();
            }

            break;

        case SYMBOL:

            node->selector += currentToken->getValue();

            break;

        case OPERATOR:
            node->selector += currentToken->getValue();
            advance();
            if ( isEnd() )
                throw CompilerError("Unexpected end of input in method signature: Missing argument name for binary message");
            node->arguments.push_back( parseArgumentName() );

            break;

        default:
            throw CompilerError("parseMethodSignature: Unexpected token: " + currentToken->toString());
        }

        return node;
    }

    std::shared_ptr<ASTNode> Parser::parse(){
        auto astRoot = parseCodeBlock();
        return astRoot;
    }

    void Parser::advance(){
        if ( ! isEnd() ){
            ++tokensIterator;
        }
    }

    Token* Parser::getNextToken(){
        if ( ! isEnd() && ++tokensIterator != tokensEnd ){
            return &(*tokensIterator);
        }else{
            return nullptr;
        }
    }

    Token* Parser::getNextTokenOrThrow(){
        if ( ! isEnd() && ++tokensIterator != tokensEnd ){
            return &(*tokensIterator);
        }else{
            throw CompilerError("getNextTokenOrThrow: Unexpected end of input");
        }
    }

    Token* Parser::getCurrentToken(){
        if ( ! isEnd() ){
            return &(*tokensIterator);
        }else{
            return nullptr;
        }
    }

    Token* Parser::peek(){
        if ( ! isEnd() ){
            auto nextIterator = std::next( tokensIterator );
            // don't deref iterator end, bad things happen if you do so.
            if ( nextIterator != tokensEnd ){
                return &(*nextIterator);
            }
        }
        return nullptr;

    }

    bool Parser::isLastToken(){
        return peek() == nullptr;
    }

    bool Parser::isEnd(){
        return tokensIterator == tokensEnd;
    }
}
