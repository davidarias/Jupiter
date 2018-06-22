// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __ARRAY_H
#define __ARRAY_H


#include <immer/flex_vector.hpp>
#include <immer/flex_vector_transient.hpp>

#include <objects/Object.hpp>


namespace jupiter{


    class Array : public Object{
    private:
        immer::flex_vector<Object*> values;

        int cmp(Object& other);
        bool equal(Object& other);
    public:
        Array();
        Array(immer::flex_vector<Object*> values);
        Array(Object** start, Object** end);

        void accept(ObjectVisitor&);

        void mark();

        Object* formatString(std::string& str);
        Object* at( int index );
        Object* push( Object* value );
        Object* take( int elems );
        Object* drop( int elems );
        Object* size();
        Object* transient();

        std::string toString();

    };

    class ArrayTransient : public Object{
    private:
        immer::flex_vector<Object*> values;
    protected:
        int cmp(Object&);
    public:
        ArrayTransient();
        ArrayTransient(immer::flex_vector<Object*> values);
        Object* push( Object* value );
        Object* persist();

        void mark();
        void accept(ObjectVisitor&);
        std::string toString();
    };


}

#endif
