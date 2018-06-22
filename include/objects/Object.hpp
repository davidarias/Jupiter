// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __OBJECT_H
#define __OBJECT_H

#include <misc/common.hpp>


namespace jupiter{

    class Object;
    class Number;
    class String;
    class Array;
    class ArrayTransient;
    class Map;
    class MapTransient;
    class Method;
    class NativeMethod;

    class ObjectVisitor{
    public:
        virtual void visit(Number&) = 0;
        virtual void visit(String&) = 0;
        virtual void visit(Array&) = 0;
        virtual void visit(ArrayTransient&) = 0;
        virtual void visit(Map&) = 0;
        virtual void visit(MapTransient&) = 0;
        virtual void visit(Method&) = 0;
        virtual void visit(NativeMethod&) = 0;
    };

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

        virtual void accept(ObjectVisitor&) = 0;
        virtual std::string toString() = 0;


    };



}
#endif
