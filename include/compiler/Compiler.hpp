// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __COMPILER_H
#define __COMPILER_H

#include "ASTNode.hpp"
#include "SymbolTable.hpp"

namespace jupiter{

    class Object;
    class CompiledMethod;
    class ConstantsTable;

    class Compiler : public ASTVisitor {

    private:
        SymbolTable locals;
        ConstantsTable& constantsTable;

        std::shared_ptr<SymbolTable> upvalues; // shared between nested compiler instances

        Compiler* enclosingMethodCompiler;
        std::shared_ptr<CompiledMethod> method;

        bool isLastNode = false;

        void addArgumentsToLocals(std::vector<std::shared_ptr<SymbolNode> >& arguments);

        // if someone is doing something crazy that will overflow the instruction size
        // we have to warn them, I find very unlikely that this happen, in any case
        // here I will leave a counter of the times that this happened:
        // CRAZY_LIMITS_REACHED_COUNTER = 0
        void checkArgumentsLimit( unsigned n );
        void checkConstantsLimit( unsigned n );
        void checkLocalsLimit( unsigned n );
        void checkLiteralArrayLimit( unsigned n );
        void checkLiteralObjectLimit( unsigned n );

        // find symbol in any outher scope and create upvalues if needed
        int createUpValuesRecursive( std::string& value );

    public:
        Compiler(ConstantsTable& constantsTable);
        Compiler(ConstantsTable& constantsTable, std::shared_ptr<MethodSignature> signature);

        Compiler(std::vector<std::shared_ptr<SymbolNode> >& arguments,
                 Compiler* enclosingMethodCompiler);

        std::shared_ptr<CompiledMethod> getCompiledMethod();

        void visit( NumberNode& node );
        void visit( SymbolNode& node );
        void visit( StringNode& node );
        void visit( ArrayNode& node );
        void visit( CodeBlockNode& node );
        void visit( AssignmentNode& node );
        void visit( MessageExpressionNode& node );
        void visit( MessageNode& node );
        void visit( CascadeMessageNode& node );
        void visit( PragmaNode& node );
        void visit( ClosureBlockNode& node );

        void compileInlineIf(MessageNode& node);
        void compileInlineBlock(std::shared_ptr<ASTNode> node);

    };

    class Primitives;
    class NativeLibraries;

    class PragmaCompiler : public ASTVisitor {
    private:
        Primitives& primitives;
        NativeLibraries& nativeLibs;

        Object* primitive;
        std::vector<std::string> arguments;
    public:
        PragmaCompiler(Primitives& primitives, NativeLibraries& nativeLibs);
        Object* getPrimitive();

        void visit( NumberNode& ){};
        void visit( SymbolNode& node );
        void visit( StringNode& node);
        void visit( ArrayNode& ){};
        void visit( CodeBlockNode& );
        void visit( AssignmentNode& ){};
        void visit( MessageExpressionNode& ){};
        void visit( MessageNode& ){};
        void visit( CascadeMessageNode& ){};
        void visit( PragmaNode& node );
        void visit( ClosureBlockNode&){};

    };
}

#endif
