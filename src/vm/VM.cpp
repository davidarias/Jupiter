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

    VM::VM(Map& globals, Map& prototypes)
        : globals(globals), prototypes(prototypes) {
        stack.push(make<Map>()); // to avoid stack underflow and crash
    }

    Object* VM::cachedTrue(){
        static auto trueObj = globals.at("true");
        return trueObj;
    }

    Object* VM::cachedFalse(){
        static auto falseObj = globals.at("false");
        return falseObj;
    }

    Object* VM::cachedNil(){
        static auto nil = globals.at("nil");
        return nil;
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
        Evaluator evaluator(*this);
        try{

            object->accept(evaluator);

        }catch (const char* s) {
            std::cout << s << std::endl;
        }catch (std::string s) {
            std::cout << s << std::endl;
        }catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }

        return stack.back();
    }

    Object* VM::eval(Method& method){
        Evaluator evaluator(*this);
        evaluator.visit(method);
        return stack.back();
    }

    void VM::pop(){
        stack.pop();
    }


    Evaluator::Evaluator(VM& vm)
        : vm(vm){}

    void Evaluator::visit(Map& obj){
        vm.stack.back(&obj);
    }

    void Evaluator::visit(MapTransient& obj){
        vm.stack.back(&obj);
    }

    void Evaluator::visit(Number& obj){
        vm.stack.back( &obj );
    }

    void Evaluator::visit(String& obj ){
        vm.stack.back( &obj );
    }

    void Evaluator::visit(Array& obj ){
        vm.stack.back( &obj );
    }

    void Evaluator::visit(ArrayTransient& obj ){
        vm.stack.back( &obj );
    }

    void Evaluator::visit(Method& obj ){

        Frame newFrame(vm, obj);
        newFrame.execute();
    }

    void Evaluator::visit(NativeMethod& method){

        auto stackSize = vm.stack.size();

        unsigned localsBaseIndex = stackSize - method.arity;
        unsigned returnIndex = localsBaseIndex - 1; // where the receiver was

        Object** args = vm.stack.begin() + localsBaseIndex;
        Object* receiver = *(args - 1); // receiver is just before the locals

        vm.stack.set( returnIndex, method.fn( receiver, args ) );
        vm.stack.resize( localsBaseIndex );
    }

    MethodAt::MethodAt(VM& vm, std::string& selector)
        : vm(vm), selector(selector){}

    Object* MethodAt::get(){
        return method;
    }

    void MethodAt::visit(Map& obj){
        method = obj.at(selector);
    }

    void MethodAt::visit(MapTransient& obj){
        static Map& behaviour = static_cast<Map&>( *(vm.prototypes.at("MapTransient")) );

        method = behaviour.at(selector);
    }

    void MethodAt::visit(Number& obj){
        static Map& behaviour = static_cast<Map&>( *(vm.prototypes.at("Number")) );

        method = behaviour.at(selector);
    }

    void MethodAt::visit(String& obj ){
        static Map& behaviour = static_cast<Map&>( *(vm.prototypes.at("String")) );

        method = behaviour.at(selector);
    }

    void MethodAt::visit(Array& obj ){
        static Map& behaviour = static_cast<Map&>( *(vm.prototypes.at("Array")) );

        method = behaviour.at(selector);
    }

    void MethodAt::visit(ArrayTransient& obj ){
        static Map& behaviour = static_cast<Map&>( *(vm.prototypes.at("ArrayTransient")) );

        method = behaviour.at(selector);
    }

    void MethodAt::visit(Method& obj ){
        static Map& behaviour = static_cast<Map&>( *(vm.prototypes.at("Method")) );

        method = behaviour.at(selector);
    }

    void MethodAt::visit(NativeMethod& method){

        throw RuntimeException("Native Methods cannot receive messages");
    }



}
