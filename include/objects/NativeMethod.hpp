// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __NATIVE_METHOD_H
#define __NATIVE_METHOD_H


#include <objects/Object.hpp>


namespace jupiter{

    typedef Object* (*NativeFunction)(Object*, Object**);

    class NativeMethod : public Object {
        friend struct Evaluator;
    private:
        NativeFunction fn;
        unsigned arity;
    protected:
        bool equal(Object& other);
        int cmp(Object&);
    public:
        NativeMethod(NativeFunction fn, unsigned arity);
        void accept(ObjectVisitor&);

        std::string toString();

    };

}

#endif
