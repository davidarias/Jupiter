// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <objects/Array.hpp>
#include <objects/Map.hpp>

#include <vm/MemoryManager.hpp>
#include <utils/format.hpp>

namespace jupiter{

    Array::Array(){}
    Array::Array( immer::flex_vector<Object*> values ) : values( values ){}

    Array::Array(Object** start, Object** end)
        : values( start, end ) {}

    void Array::accept(ObjectVisitor& visitor){
        visitor.visit(*this);
    }

    void Array::mark(){
        marked = true;
        for(auto v : values){
            v->mark();
        }
    }

    Object* Array::push( Object* value ){
        return make<Array>( values.push_back(value) );
    }

    Object* Array::take( int elems ){
        return make<Array>( values.take( elems ) );
    }

    Object* Array::drop( int elems ){
        return make<Array>( values.drop( elems ) );
    }

    Object* Array::size(){
        return make<Number>( values.size() );
    }

    Object* Array::formatString(std::string& str){
        return make<String>( format( str, values ) );
    }

    Object* Array::at( int index ){
        return values.at( index - 1 ); // arrays starts at index 1
    }

    bool Array::equal(Object& other){
        // we checked the type in the == operator
        auto otherArray = static_cast<Array&>(other);
        if ( values.size() != otherArray.values.size() ) return false;

        auto it1 = values.begin();
        auto it1End = values.end();

        auto it2 = otherArray.values.begin();
        auto it2End = otherArray.values.end();

        while( it1 != it1End && it2 != it2End ){
            Object& a = *(*it1);
            Object& b = *(*it2);

            if ( a == b ){
                ++it1; ++it2;
                continue;
            }else{
                return false;
            }
        }

        return true;
    }

    template<class T>
    int cmpimmerVector(T values, T other){

        auto it1 = values.begin();
        auto it1End = values.end();

        auto it2 = other.begin();
        auto it2End = other.end();

        while( it1 != it1End && it2 != it2End ){
            Object& a = *(*it1);
            Object& b = *(*it2);

            if ( a < b ){
                return -1;
            }else if( a > b){
                return 1;
            }
            ++it1; ++it2;
        }

        // for now all values are the same, check the size to return
        if ( values.size() == other.size() ){
            return 0;
        }else if ( values.size() < other.size() ){
            return -1;
        }else{
            return 1;
        }
    }

    int Array::cmp(Object& other){
        // we checked the type in the == operator
        auto otherArray = static_cast<Array&>(other);

        return cmpimmerVector(values, otherArray.values );
    }

    std::string Array::toString(){
        std::ostringstream buffer;
        buffer << "Array " << this;
        return buffer.str();
    }


    Object* Array::transient(){
        return make<ArrayTransient>( values );
    }

    ArrayTransient::ArrayTransient() {}
    ArrayTransient::ArrayTransient(immer::flex_vector<Object*> values) :
        values( values.transient() ) {}

    Object* ArrayTransient::push( Object* value){
        // transients can point to younger objects
        // breaking the invariant of innmutable containers
        // ( adding objects will make new containers)
        // to avoid invalid collection of elements
        // on minor cycles (since this object can be tenured
        // its elements won't be marked )
        // we mark all objects added as a precaution
        // making them tenured
        value->mark();
        values.push_back( value );
        return this;
    }

    Object* ArrayTransient::persist(){
        return make<Array>( values.persistent() );
    }

    std::string ArrayTransient::toString(){
        std::ostringstream buffer;
        buffer << "Array Transient" << this;
        return buffer.str();
    }

    void ArrayTransient::accept(ObjectVisitor& visitor){
        visitor.visit(*this);
    }

    int ArrayTransient::cmp(Object& other){
        // we checked the type in the == operator
        auto otherArray = static_cast<ArrayTransient&>(other);

        return cmpimmerVector(values, otherArray.values );
    }

    void ArrayTransient::mark(){
        marked = true;
        for(auto v : values){
            v->mark();
        }
    }


}
