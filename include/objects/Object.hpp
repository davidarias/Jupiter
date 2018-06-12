// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __OBJECT_H
#define __OBJECT_H

#include <misc/common.hpp>


namespace jupiter{

    class Object;

    class GCObject{
    protected:
        bool marked = false;
        bool tenured = false;
    public:
        virtual void mark();
        virtual void unmark();
        bool isMarked();

        bool istenured();
        void setTenured();

    };

    class Evaluator;

    bool operator==(Object& a, Object& b);
    bool operator!=(Object& a, Object& b);
    bool operator>(Object& a, Object& b);
    bool operator<(Object& a, Object& b);
    bool operator<=(Object& a, Object& b);
    bool operator>=(Object& a, Object& b);

    class Object : public GCObject {

    protected:
        friend bool operator==(Object& a, Object& b);
        friend bool operator!=(Object& a, Object& b);
        friend bool operator>(Object& a, Object& b);
        friend bool operator<(Object& a, Object& b);
        friend bool operator<=(Object& a, Object& b);
        friend bool operator>=(Object& a, Object& b);

        virtual int cmp(Object& other) = 0;
        virtual bool equal(Object& other);
    public:

        Object();
        virtual ~Object();

        virtual Object* at(const std::string& selector) = 0;
        virtual std::string toString() = 0;
        virtual void eval(Evaluator& evaluator) = 0;


    };



}
#endif
