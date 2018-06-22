// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __VM_H
#define __VM_H

#include <misc/common.hpp>
#include <vm/Stack.hpp>
#include <objects/Objects.hpp>

namespace jupiter{

    class Object;
    class Map;
    class Number;
    class String;
    class Array;
    class Method;

    class Frame;

    class VM{
        friend class Frame;
        friend class Evaluator;
        friend class MethodAt;
    private:
        Stack stack;
        Map& globals;
        Map& prototypes;

        Object* cachedTrue();
        Object* cachedFalse();
        Object* cachedNil();
    public:
        VM(Map& globals, Map& prototypes);

        void mark(bool full);

        void pop();

        Object* eval(Object* object);
        Object* eval(Method& method);

    };

    class Evaluator : public ObjectVisitor{
    private:
        VM& vm;
    public:
        Evaluator(VM& vm);
        void visit(Map&);
        void visit(MapTransient&);
        void visit(Number&);
        void visit(String&);
        void visit(Array&);
        void visit(ArrayTransient&);
        void visit(Method&);
        void visit(NativeMethod&);

    };

    class MethodAt : public ObjectVisitor{
    private:
        VM& vm;
        std::string& selector;
        Object* method;
    public:
        MethodAt(VM& vm, std::string& selector);

        Object* get();

        void visit(Map&);
        void visit(MapTransient&);
        void visit(Number&);
        void visit(String&);
        void visit(Array&);
        void visit(ArrayTransient&);
        void visit(Method&);
        void visit(NativeMethod&);

    };
}
#endif
