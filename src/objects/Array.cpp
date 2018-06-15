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

    Object* Array::at(const std::string& selector){
        // globals are always maps
        static Map& behaviour =  static_cast<Map&>( *( World::instance().prototypes.at("Array") ));
        return behaviour.at( selector );
    }

    Object* Array::push( Object* value ){
        return MemoryManager<Array>::instance().get( values.push_back(value) );
    }

    Object* Array::take( int elems ){
        return MemoryManager<Array>::instance().get( values.take( elems ) );
    }

    Object* Array::drop( int elems ){
        return MemoryManager<Array>::instance().get( values.drop( elems ) );
    }

    Object* Array::size(){
        return MemoryManager<Number>::instance().get( values.size() );
    }

    Object* Array::formatString(std::string& str){
        return MemoryManager<String>::instance().get( format( str, values ) );
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

    int Array::cmp(Object& other){
        // we checked the type in the == operator
        auto otherArray = static_cast<Array&>(other);

        auto it1 = values.begin();
        auto it1End = values.end();

        auto it2 = otherArray.values.begin();
        auto it2End = otherArray.values.end();

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
        if ( values.size() == otherArray.values.size() ){
            return 0;
        }else if ( values.size() < otherArray.values.size() ){
            return -1;
        }else{
            return 1;
        }
    }

    std::string Array::toString(){
        std::ostringstream buffer;
        buffer << "Array " << this;
        return buffer.str();
    }


    Object* Array::transient(){
        return MemoryManager<ArrayTransient>::instance().get( values );
    }

    ArrayTransient::ArrayTransient() {}
    ArrayTransient::ArrayTransient(immer::flex_vector<Object*> values) : Array( values ) {}

    Object* ArrayTransient::at(const std::string& selector){

        static Map& behaviour = static_cast<Map&>( *( World::instance().prototypes.at("ArrayTransient") ));
        return behaviour.at( selector );

    }

    Object* ArrayTransient::push( Object* value){
        // transients can point to young objects
        // being tenured, so when adding an object
        // to a tenured transient we also mark
        // this object as tenured. when minor gc cycle
        // is executed, we move this object to tenured space
        if ( istenured() ){
            value->setTenured();
        }
        values = std::move(values).push_back( value );
        return this;
    }

    Object* ArrayTransient::persist(){
        return MemoryManager<Array>::instance().get( values );
    }


}
