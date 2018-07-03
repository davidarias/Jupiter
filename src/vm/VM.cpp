// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/VM.hpp>
#include <vm/Frame.hpp>

#include <memory/memory.hpp>
#include <vm/Stack.hpp>
#include <vm/World.hpp>
#include <vm/ConstantsTable.hpp>

#include <objects/Object.hpp>
#include <objects/CompiledMethod.hpp>

#include <primitives/primitives.hpp>

#include <misc/Exceptions.hpp>

namespace jupiter{

    VM::VM(World& world) : world(world) {

        stack.push(make<Map>()); // to avoid stack underflow and crash
    }

    void VM::mark(bool full){

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

        }catch(SelectorNotFound& e){

            std::string selector = world.constantsTable.get(e.key)->toString();
            std::cout << "Selector '" << selector << "' not found "<< std::endl;

        }catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }

        return stack.back();
    }

    Object* VM::eval(Method& method){
        Evaluator evaluator(*this);
        try{
            evaluator.visit(method);
        }catch(SelectorNotFound& e){

            std::string selector = world.constantsTable.get(e.key)->toString();
            std::cout << "Selector '" << selector << "' not found "<< std::endl;

        }catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }

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

        vm.stack.set( returnIndex, method.fn( &(vm.world), receiver, args ) );
        vm.stack.resize( localsBaseIndex );
    }

    void Evaluator::visit(UserData& obj){
        vm.stack.back( &obj );
    }

    MethodAt::MethodAt(VM& vm, unsigned selector)
        : vm(vm), selector(selector){}

    Object* MethodAt::get(){
        return method;
    }

    void MethodAt::visit(Map& obj){
        method = obj.at(selector);
    }

    void MethodAt::visit(MapTransient&){
        static Map& behaviour = static_cast<Map&>( *(vm.world.getPrototype("MapTransient")) );

        method = behaviour.at(selector);
    }

    void MethodAt::visit(Number&){
        static Map& behaviour = static_cast<Map&>( *(vm.world.getPrototype("Number")) );

        method = behaviour.at(selector);
    }

    void MethodAt::visit(String& ){
        static Map& behaviour = static_cast<Map&>( *(vm.world.getPrototype("String")) );

        method = behaviour.at(selector);
    }

    void MethodAt::visit(Array& ){
        static Map& behaviour = static_cast<Map&>( *(vm.world.getPrototype("Array")) );

        method = behaviour.at(selector);
    }

    void MethodAt::visit(ArrayTransient& ){
        static Map& behaviour = static_cast<Map&>( *(vm.world.getPrototype("ArrayTransient")) );

        method = behaviour.at(selector);
    }

    void MethodAt::visit(Method& ){
        static Map& behaviour = static_cast<Map&>( *(vm.world.getPrototype("Method")) );

        method = behaviour.at(selector);
    }

    void MethodAt::visit(NativeMethod&){

        throw RuntimeException("Native Methods cannot receive messages");
    }

    void MethodAt::visit(UserData&){
        throw RuntimeException("User data cannot receive messages");
    }



}
