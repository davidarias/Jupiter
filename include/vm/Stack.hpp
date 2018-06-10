// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __STACK_H
#define __STACK_H

#include <misc/common.hpp>
#include <objects/Objects.hpp>

namespace jupiter{

    class Object;

    class Stack{
    private:
        Object** first;
        Object** last;
        size_t _capacity;

        Map dummy; // to insert at empty spaces

    public:
        Stack();
        ~Stack();

        void push(Object* obj);
        Object* pop();
        Object* back();
        Object* back(Object* value);
        void dup();

        unsigned size();
        unsigned capacity();
        void resize(unsigned newSize);
        void clear();

        Object* get(unsigned index);
        void set(unsigned index, Object* value);

        Object** begin();
        Object** end();

        void printStack();


    };


}
#endif
