// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/Stack.hpp>

#include <objects/Object.hpp>

namespace jupiter{


    Stack::Stack() {
        stack.reserve(1024);
    }

    void Stack::push(Object* obj){
        stack.push_back(obj);
    }

    Object* Stack::pop(){
        auto returnValue = stack.back();
        stack.pop_back();
        return returnValue;
    }

    Object* Stack::back(){
        return stack.back();
    }

    Object* Stack::back(Object* value){
        return stack.back() = value;
    }

    void Stack::dup(){
        stack.push_back( stack.back() );
    }

    unsigned Stack::size(){
        return stack.size();
    }

    unsigned Stack::capacity(){
        return stack.capacity();
    }

    void Stack::resize(unsigned newSize){
        stack.resize( newSize );
    }

    void Stack::clear(){
        stack.clear();
    }

    Object* Stack::get(unsigned index){
        return stack[index];
    }

    void Stack::set(unsigned index, Object* value){
        stack[index] = value;
    }

    std::vector<Object*>::iterator Stack::begin(){
        return stack.begin();
    }

    std::vector<Object*>::iterator Stack::end(){
        return stack.end();
    }

    void Stack::printStack(){
        LOG( "------------------" );
        LOG("-- DEBUG STACK   --");
        LOG( "------------------" );
        LOG( "" );

        auto it = stack.begin();
        int counter = 0;
        for(; it != stack.end(); it++){
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
