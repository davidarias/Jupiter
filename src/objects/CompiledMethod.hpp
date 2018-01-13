// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __BYTECODE_H
#define __BYTECODE_H

#include <misc/common.hpp>

namespace jupiter{
    // forward declarations
    class Object;
    class Method;

    class VM;
    class ExecutionFrame;

    enum Bytecode {
        PUSH_CONSTANT,
        PUSH_LOCAL,
        PUSH_GLOBAL,
        PUSH_SELF,
        PUSH_CLOSURE,
        PUSH_UPVALUE,
        POP_INTO_UPVALUE,
        POP_INTO,
        POP,
        POP_N_INTO_ARRAY,
        POP_N_INTO_OBJECT,
        RETURN_TOP,
        DUP,
        SEND
    };

    struct Instruction{
        uint8_t bytecode;
        uint16_t argument;
        uint8_t shortArgument;
    };


    class CompiledMethod{
        friend class VM;
        friend class ExecutionFrame;
    private:
        unsigned locals; // includes arguments
        unsigned arity;
        std::vector<Method*> closures;
        std::vector<Instruction> instructions;
        std::vector<std::pair<unsigned, unsigned> > upvalues; // enclosing context local index

    public:
        // noOp for default method constructor, so if an exception happen in compilation,
        // execution of method does not crash
        static CompiledMethod& noOp() {
            static CompiledMethod m;
            return m;
        }

        CompiledMethod();

        unsigned addClosure( Method* value );

        void addInstruction(Bytecode code, uint16_t argument, uint8_t shortArgument);
        void addInstruction(Bytecode code, uint16_t argument);
        void addInstruction(Bytecode code);

        void setLocals( int _locals );
        void setArity( int _arity );

        void addUpValue(unsigned upvalueIndex, unsigned enclosingLocalIndex );
        bool isUpvalueInitialized(unsigned upvalueIndex );

        void printBytecode();
    };

}
#endif
