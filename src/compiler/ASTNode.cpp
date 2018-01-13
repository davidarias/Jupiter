// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <compiler/ASTNode.hpp>

namespace jupiter{

    ASTNode::ASTNode(){}

    NumberNode::NumberNode( std::string& value ) : value( value ){}
    SymbolNode::SymbolNode( std::string& value ) : value( value ){}
    StringNode::StringNode( std::string& value ) : value( value ){}
    ArrayNode::ArrayNode(){}
    ObjectLiteralNode::ObjectLiteralNode(){}
    CodeBlockNode::CodeBlockNode(){}
    AssignmentNode::AssignmentNode(){}
    MessageExpressionNode::MessageExpressionNode( ){}
    MessageNode::MessageNode( ){}
    CascadeMessageNode::CascadeMessageNode( ){}
    PragmaNode::PragmaNode(){}
    ClosureBlockNode::ClosureBlockNode(){}
    MethodSignature::MethodSignature(){}
}
