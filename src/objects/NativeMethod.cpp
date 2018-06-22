// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <objects/NativeMethod.hpp>

#include <vm/World.hpp>

namespace jupiter{

    NativeMethod::NativeMethod(NativeFunction fn, unsigned arity) :
        fn(fn), arity(arity) {}

    void NativeMethod::accept(ObjectVisitor& visitor){
        visitor.visit(*this);
    }

    bool NativeMethod::equal(Object& other){
        if (&other == this) return true;
        return false;
    }

    int NativeMethod::cmp(Object&){
        throw "Primitive methods cannot be compared";
    }

    std::string NativeMethod::toString(){
        std::stringstream buffer;
        buffer << "Primitive Method " << this;
        return buffer.str();
    }

}
