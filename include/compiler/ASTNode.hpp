// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __ASTNODE_H
#define __ASTNODE_H

#include <misc/common.hpp>

namespace jupiter{

    class ASTNode;
    class NumberNode;
    class SymbolNode;
    class StringNode;
    class ArrayNode;
    class ObjectLiteralNode;
    class CodeBlockNode;
    class AssignmentNode;
    class MessageExpressionNode;
    class MessageNode;
    class CascadeMessageNode;
    class PragmaNode;
    class ClosureBlockNode;

    class ASTVisitor {

    public:
        virtual void visit( NumberNode& ) = 0;
        virtual void visit( SymbolNode& ) = 0;
        virtual void visit( StringNode& ) = 0;
        virtual void visit( ArrayNode& ) = 0;
        virtual void visit( ObjectLiteralNode& ) = 0;
        virtual void visit( CodeBlockNode& ) = 0;
        virtual void visit( AssignmentNode& ) = 0;
        virtual void visit( MessageExpressionNode& ) = 0;
        virtual void visit( MessageNode& ) = 0;
        virtual void visit( CascadeMessageNode& ) = 0;
        virtual void visit( PragmaNode& ) = 0;
        virtual void visit( ClosureBlockNode& ) = 0;

    };

    class ASTNode{

    public:
        ASTNode();

        virtual void accept(ASTVisitor& visitor) = 0;
    };

    class NumberNode : public ASTNode {
    public:
        std::string value;

        NumberNode( std::string& value );

        void accept(ASTVisitor& visitor) {
            visitor.visit(*this);
        }

    };

    class SymbolNode : public ASTNode {
    public:
        std::string value;

        SymbolNode( std::string& value );

        void accept(ASTVisitor& visitor) {
            visitor.visit(*this);
        }
    };

    class StringNode : public ASTNode {
    public:
        std::string value;

       StringNode( std::string& value );

        void accept(ASTVisitor& visitor) {
            visitor.visit(*this);
        }
    };

    class ArrayNode : public ASTNode {
    public:
        std::vector<std::shared_ptr<ASTNode> > values;

        ArrayNode();

        void accept(ASTVisitor& visitor) {
            visitor.visit(*this);
        }
    };

    class ObjectLiteralNode : public ASTNode {
    public:
        std::vector<std::string> keys;
        std::vector<std::shared_ptr<ASTNode> > values;

        ObjectLiteralNode();

        void accept(ASTVisitor& visitor) {
            visitor.visit(*this);
        }
    };


    class CodeBlockNode : public ASTNode {
    public:
        std::vector<std::shared_ptr<ASTNode> > nodes;

        CodeBlockNode();

        void accept(ASTVisitor& visitor) {
            visitor.visit(*this);
        }
    };


    class AssignmentNode : public ASTNode {
    public:
        std::shared_ptr<SymbolNode> symbol;
        std::shared_ptr<ASTNode> value;

        AssignmentNode();

        void accept(ASTVisitor& visitor) {
            visitor.visit(*this);
        }
    };

    class MessageExpressionNode : public ASTNode {
    public:
        std::shared_ptr<ASTNode> message;

        MessageExpressionNode();

        void accept(ASTVisitor& visitor) {
            visitor.visit(*this);
        }
    };

    class MessageNode : public ASTNode {
    public:
        std::shared_ptr<ASTNode> receiver;
        std::string selector;
        std::vector<std::shared_ptr<ASTNode> > arguments;

        MessageNode();

        void accept(ASTVisitor& visitor) {
            visitor.visit(*this);
        }
    };

    class CascadeMessageNode : public ASTNode {
    public:
         // caution! only the first message has a valid receiver
        std::vector<std::shared_ptr<MessageNode> > messages;

        CascadeMessageNode();

        void accept(ASTVisitor& visitor) {
            visitor.visit(*this);
        }
    };

    class PragmaNode : public ASTNode {
    public:
        std::string selector;
        std::vector<std::shared_ptr<ASTNode> > arguments;

        PragmaNode();

        void accept(ASTVisitor& visitor) {
            visitor.visit(*this);
        }
    };

    class ClosureBlockNode : public ASTNode {
    public:

        std::vector<std::shared_ptr<SymbolNode> > arguments;
        std::shared_ptr<CodeBlockNode> code;

        ClosureBlockNode();

        void accept(ASTVisitor& visitor) {
            visitor.visit(*this);
        }
    };

    class MethodSignature {
    public:
        std::string selector;
        std::vector<std::shared_ptr<SymbolNode> > arguments;

        MethodSignature();

    };



}

#endif
