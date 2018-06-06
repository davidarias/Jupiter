// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __OBJECT_H
#define __OBJECT_H

#include <misc/common.hpp>


#include <immer/flex_vector.hpp>
#include <immer/flex_vector_transient.hpp>

#include <immer/box.hpp>
#include <immer/map.hpp>


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

    class Map: public Object{
    protected:

        int cmp(Object&);
        bool equal(Object& other);
    public:
        immer::map<std::string, Object* > slots;

        Map();
        Map(immer::map<std::string, Object* > slots);

        void mark();

        std::string toString();
        void eval(Evaluator& evaluator);

        Object* at(const std::string& key);
        Object* putAt(const std::string& key, Object* value);
        void putAtMut(const std::string& key, Object* value);

        Object* transient();
    };

    class MapTransient : public Map{
    public:
        MapTransient();
        MapTransient(immer::map<std::string, Object* > slots);
        Object* at(const std::string& key);
        void putAt(const std::string& key, Object* value);
        Object* persist();
    };



    class Array : public Object{
    protected:
        immer::flex_vector<Object*> values;

        int cmp(Object& other);
        bool equal(Object& other);
    public:
        Array();
        Array(immer::flex_vector<Object*> values);
        Array(std::vector<Object*>::iterator start, std::vector<Object*>::iterator end);

        void mark();

        Object* formatString(std::string& str);
        Object* at( int index );
        Object* push( Object* value );
        Object* take( int elems );
        Object* drop( int elems );
        Object* size();
        Object* transient();

        Object* at(const std::string& selector);
        std::string toString();
        void eval(Evaluator& evaluator);

    };

    class ArrayTransient : public Array{
    public:
        ArrayTransient();
        ArrayTransient(immer::flex_vector<Object*> values);
        Object* at(const std::string& key);
        Object* push( Object* value );
        Object* persist();
    };


    class CompiledMethod;

    class Method : public Object {
        friend class ExecutionFrame;
    private:
        std::string name;
        std::string signature;
        std::string source;
        std::shared_ptr<CompiledMethod> compiledMethod;
        Object* self;

        // TODO optimize this ( maybe using a sparse vector? )
        std::unordered_map<unsigned, Object*> upvalues;

    protected:
        int cmp(Object& other);
        bool equal(Object& other);
    public:
        Method();
        Method(std::string& name, std::string& signature, std::string& source,
               std::shared_ptr<CompiledMethod> compiledMethod);

        Method(std::shared_ptr<CompiledMethod> compiledMethod);
        ~Method();

        void mark();

        std::string& getName();
        std::shared_ptr<CompiledMethod> getCompiledMethod();

        Object* at(const std::string& selector);
        std::string toString();
        void eval(Evaluator& evaluator);


    };

    class PrimitiveArguments;
    typedef Object* (*PrimitiveFunction)(PrimitiveArguments&);

    class PrimitiveMethod : public Object {
        friend struct Evaluator;
    private:
        PrimitiveFunction fn;
        unsigned arity;
    protected:
        bool equal(Object& other);
        int cmp(Object&);
    public:
        PrimitiveMethod(PrimitiveFunction fn, unsigned arity);

        Object* at(const std::string& selector);
        std::string toString();
        void eval(Evaluator&);

    };


}
#endif
