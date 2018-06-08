// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __VM_H
#define __VM_H

#include <misc/common.hpp>
#include <vm/Stack.hpp>

namespace jupiter{

    class Object;
    class Map;
    class Number;
    class String;
    class Array;
    class Method;

    class NativeMethod;

    class CompiledMethod;
    class UpValue;

    struct Instruction;

    class ExecutionFrame{
    private:
        Stack& stack;
        Method& method;
        std::shared_ptr<CompiledMethod> compiledMethod;
        Object* self;
        // where are located the locals in the stack
        unsigned localsBaseIndex;
        unsigned returnIndex;

        unsigned instructionCounter;

        std::string& getStringConstant( unsigned id);
        Object* getLocal( unsigned index );
        unsigned getLocalIndex( unsigned index );

        void pushConstant( unsigned id );
        void pushLocal( unsigned id );
        void pushGlobal( unsigned id );
        void pushSelf();
        void pushClosure( unsigned id );
        void pushUpValue( unsigned id );
        void popInto( unsigned id );
        void pop();
        void popNIntoArray( unsigned n );
        void dup();
        void send( uint16_t id, uint8_t receiverRelPos );
        void jumpIfFalse( uint16_t id );
        void jumpIfTrue( uint16_t id );
        void jump( uint16_t id );

        void dispatch( Instruction instruction );

    public:
        ExecutionFrame(Stack& stack, Method& method, Object* self);
        ~ExecutionFrame();
        void execute();
    };

    class VM{
    private:
        Stack stack;
    public:
        VM();

        void gc(bool full);

        void pop();

        Object* eval(Object* object);
        Object* eval(Object* object, Object* self);
        Object* eval(Method& method, Object* self);

    };

    class Evaluator{
    private:
        Object* receiver;
        Stack& stack;
    public:
        Evaluator(Object* receiver, Stack& stack);
        void operator()(Map&);
        void operator()(Number&);
        void operator()(String&);
        void operator()(Array&);
        void operator()(Method&);
        void operator()(NativeMethod&);

    };
}
#endif
