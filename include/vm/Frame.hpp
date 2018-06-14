// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __FRAME_H
#define __FRAME_H

#include <misc/common.hpp>
#include <vm/Stack.hpp>
#include <vm/VM.hpp>

namespace jupiter{

    class Object;
    class Map;
    class Number;
    class String;
    class Array;
    class Method;

    class NativeMethod;
    class CompiledMethod;

    struct Instruction;

    class Frame{
    private:
        VM& vm;
        Map& globals;
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
        Frame(VM& vm, Method& method);
        ~Frame();
        void execute();
    };
}
#endif
