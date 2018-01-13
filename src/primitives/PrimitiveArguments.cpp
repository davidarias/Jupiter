// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <primitives/PrimitiveArguments.hpp>

#include <vm/Stack.hpp>

namespace jupiter{

    PrimitiveArguments::PrimitiveArguments(Stack& stack,
                                           unsigned arity,
                                           unsigned baseIndex,
                                           Object* receiver) :
        stack( stack ), arity( arity ), baseIndex ( baseIndex ), receiver(receiver) {}

    Object* PrimitiveArguments::get(unsigned index){
        if ( index > ( arity -1 ) ) throw "Primitive Arguments: index out of range";
        return stack.get( index + baseIndex );
    }

    Object* PrimitiveArguments::getReceiver(){
        return receiver;
    }

    unsigned PrimitiveArguments::size(){
        return arity;
    }
}
