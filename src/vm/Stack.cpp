// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/Stack.hpp>

#include <objects/Object.hpp>

namespace jupiter{


    Stack::Stack() {
        _capacity = 128;
        auto newMem = std::malloc(sizeof(Object*) * _capacity );
        if ( newMem == nullptr) throw std::bad_alloc();
        first = reinterpret_cast<Object**>( newMem );
        last = first;
    }

    Stack::~Stack() {

        std::free(first);

    }

    void Stack::push(Object* obj){
        *last = obj;
        last++;
    }

    Object* Stack::pop(){
        last--;
        return *last;
    }

    Object* Stack::back(){
        return *(last - 1);
    }

    Object* Stack::back(Object* value){
        *(last - 1) = value;
        return value;
    }

    void Stack::dup(){
        push( back() );
    }

    unsigned Stack::size(){
        return last - first;
    }

    unsigned Stack::capacity(){
        return _capacity;
    }

    void Stack::resize(unsigned newSize){
        if ( newSize >= _capacity){
            auto oldSize = size();
            _capacity *= 2;
            auto newMem = std::realloc(first, sizeof(Object*) * _capacity );
            if ( newMem == nullptr) throw std::bad_alloc();
            first = reinterpret_cast<Object**>( newMem );
            last = first + oldSize -1;
            return;
        }
        last = first + newSize;
    }

    void Stack::clear(){
        last = first;
    }

    Object* Stack::get(unsigned index){
        return first[index];
    }

    void Stack::set(unsigned index, Object* value){
        first[index] = value;
    }

    Object** Stack::begin(){
        return first;
    }

    Object** Stack::end(){
        return last;
    }

    void Stack::printStack(){
        LOG( "------------------" );
        LOG("-- DEBUG STACK   --");
        LOG( "------------------" );
        LOG( "" );

        auto it = begin();
        int counter = 0;
        for(; it != end(); it++){
            auto obj = (*it);
            LOG_INLINE(" " << counter);
            if ( obj != nullptr ){
                LOG( obj->toString() );
            }else{
                LOG("nullptr");
            }
            counter++;
        }
    }

}
