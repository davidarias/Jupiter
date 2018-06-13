// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/VM.hpp>
#include <vm/Frame.hpp>

#include <vm/MemoryManager.hpp>
#include <vm/Stack.hpp>
#include <vm/World.hpp>
#include <vm/ConstantsTable.hpp>

#include <objects/Object.hpp>
#include <objects/CompiledMethod.hpp>

#include <primitives/primitives.hpp>

#include <misc/Exceptions.hpp>

namespace jupiter{

    VM::VM(){
        stack.push(MemoryManager<Map>::instance().get()); // to avoid stack underflow and crash
    }

    void VM::gc(bool full){

        if ( full ){
            for( auto it = stack.begin(); it != stack.end(); ++it){
                (*it)->mark();
            }
        }else{
            for( auto it = stack.begin(); it != stack.end(); ++it){
                if ( ! (*it)->istenured() ){
                    (*it)->mark();
                }
            }
        }

    }

    Object* VM::eval(Object* object){
        return eval(object, nullptr);
    }

    Object* VM::eval(Object* object, Object* self){
        Evaluator evaluator(self, stack);
        try{

            object->eval(evaluator);

        }catch (const char* s) {
            std::cout << s << std::endl;
        }catch (std::string s) {
            std::cout << s << std::endl;
        }catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }

        return stack.back();
    }

    Object* VM::eval(Method& method, Object* self){
        Evaluator evaluator(self, stack);
        evaluator(method);
        return stack.back();
    }

    void VM::pop(){
        stack.pop();
    }


    Evaluator::Evaluator(Object* receiver, Stack& stack) : receiver(receiver), stack(stack){}

    void Evaluator::operator()(Map& obj){
        stack.back(&obj);
    }

    void Evaluator::operator()(Number& obj){
        stack.back( &obj );
    }

    void Evaluator::operator()(String& obj ){
        stack.back( &obj );
    }

    void Evaluator::operator()(Array& obj ){
        stack.back( &obj );
    }

    void Evaluator::operator()(Method& obj ){

        Frame newFrame(stack, obj, receiver );
        newFrame.execute();
    }

    void Evaluator::operator()(NativeMethod& method){

        auto stackSize = stack.size();

        unsigned localsBaseIndex = stackSize - method.arity;
        unsigned returnIndex = localsBaseIndex - 1; // where the receiver was

        Object** args = stack.begin() + localsBaseIndex;

            stack.set( returnIndex, method.fn( receiver, args ) );
        stack.resize( localsBaseIndex );
    }



}
