// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __PRIMITIVE_H
#define __PRIMITIVE_H

#include <misc/common.hpp>

namespace jupiter{

    class Object;
    class Stack;

    // helper class for primitives
    class PrimitiveArguments{
    public:
        PrimitiveArguments(Stack& stack, unsigned arity, unsigned baseIndex, Object* receiver );

        Object* get(unsigned index);
        Object* getReceiver();
        unsigned size();
    private:
        Stack& stack;
        unsigned arity, baseIndex;
        Object* receiver;
    };


}

#endif
