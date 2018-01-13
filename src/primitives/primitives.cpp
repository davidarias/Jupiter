// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <primitives/primitives.hpp>

namespace jupiter{

    Primitives::Primitives(){
        primitives.reserve(22);

        // common
        add("print",            PrimitiveMethod(print, 1) );
        add("endl",            PrimitiveMethod(endl,0) );
        add("isIdenticalTo",    PrimitiveMethod(isIdenticalTo, 1) );
        add("equals",   PrimitiveMethod(equals, 1) );
        add("greater",   PrimitiveMethod(greater, 1) );
        add("greaterOrEqual",   PrimitiveMethod(greaterOrEqual, 1) );
        add("less",   PrimitiveMethod(less, 1) );
        add("lessOrEqual",   PrimitiveMethod(lessOrEqual, 1) );

        // numbers

        add("plus",           PrimitiveMethod(plus, 1) );
        add("minus",           PrimitiveMethod(minus, 1) );
        add("multiply",       PrimitiveMethod(multiply, 1) );
        add("divide",       PrimitiveMethod(divide, 1) );

        // strings

        // arrays
        add("arrayAt",             PrimitiveMethod(arrayAt, 1) );
        add("arrayPush",             PrimitiveMethod(arrayPush, 1) );
        add("arraySize",             PrimitiveMethod(arraySize, 0) );
        add("arrayFormatString",   PrimitiveMethod(arrayFormatString, 1) );

        add("arrayTransient",  PrimitiveMethod(arrayTransient, 0) );
        add("arrayTransientPersist",  PrimitiveMethod(arrayTransientPersist, 0) );
        add("arrayTransientPush",  PrimitiveMethod(arrayTransientPush, 1) );

        // maps
        add("mapAt",      PrimitiveMethod(mapAt, 1) );
        add("mapAtPut",      PrimitiveMethod(mapAtPut, 2) );
        add("mapTransient",  PrimitiveMethod(mapTransient, 0) );
        add("mapTransientPersist",  PrimitiveMethod(mapTransientPersist, 0) );
        add("mapTransientAtPut",  PrimitiveMethod(mapTransientAtPut, 2) );

        // methods
        add("eval0",  PrimitiveMethod(methodEval, 0) );
        add("eval1",  PrimitiveMethod(methodEval, 1) );
        add("eval2",  PrimitiveMethod(methodEval, 2) );
        add("eval3",  PrimitiveMethod(methodEval, 3) );
        add("printBytecode",  PrimitiveMethod(methodPrintByteCode, 0) );
    }

    Primitives::~Primitives(){}

    void Primitives::add(std::string name, PrimitiveMethod primitive){
        unsigned index = primitives.size();
        primitives.push_back(primitive);
        primitivesMap[name] = index;
    }

    Object* Primitives::get(std::string& name){
        try{
            return &( primitives.at( primitivesMap.at(name) ) );
        }catch(std::exception& e){
            throw  "Primitive \'" + name + "\' not found.";
        }

    }

        Object* print(PrimitiveArguments& arguments){

        std::cout << arguments.get(0)->toString();
        return arguments.getReceiver();
    }

    Object* endl(PrimitiveArguments& arguments){

        std::cout << std::endl;
        return arguments.getReceiver();
    }


    Object* equals(PrimitiveArguments& arguments){

        if ( *( arguments.getReceiver() ) == *( arguments.get(0) ) ){
            return World::instance().getTrue();
        }else{
            return World::instance().getFalse();
        }
    }


    Object* greater(PrimitiveArguments& arguments){

        if ( *( arguments.getReceiver() ) > *( arguments.get(0) ) ){
            return World::instance().getTrue();
        }else{
            return World::instance().getFalse();
        }
    }


    Object* less(PrimitiveArguments& arguments){

        if ( *( arguments.getReceiver() ) < *( arguments.get(0) ) ){
            return World::instance().getTrue();
        }else{
            return World::instance().getFalse();
        }
    }


    Object* greaterOrEqual(PrimitiveArguments& arguments){

        if ( *( arguments.getReceiver() ) >= *( arguments.get(0) ) ){
            return World::instance().getTrue();
        }else{
            return World::instance().getFalse();
        }
    }


    Object* lessOrEqual(PrimitiveArguments& arguments){

        if ( *( arguments.getReceiver() ) <= *( arguments.get(0) ) ){
            return World::instance().getTrue();
        }else{
            return World::instance().getFalse();
        }
    }


    Object* isIdenticalTo(PrimitiveArguments& arguments){

        if ( arguments.getReceiver() == arguments.get(0) ){
            return World::instance().getTrue();
        }else{
            return World::instance().getFalse();
        }
    }

    Object* plus(PrimitiveArguments& arguments){

        Number& self = dynamic_cast<Number&>( *( arguments.getReceiver() ) );
        Number& arg0 = dynamic_cast<Number&>( *( arguments.get(0) ) );

        return  self + arg0;

    }

    Object* minus(PrimitiveArguments& arguments){

        Number& self = dynamic_cast<Number&>( *( arguments.getReceiver() ) );
        Number& arg0 = dynamic_cast<Number&>( *( arguments.get(0) ) );

        return  self - arg0;

    }

    Object* multiply(PrimitiveArguments& arguments){

        Number& self = dynamic_cast<Number&>( *( arguments.getReceiver() ) );
        Number& arg0 = dynamic_cast<Number&>( *( arguments.get(0) ) );

        return  self * arg0;

    }

    Object* divide(PrimitiveArguments& arguments){

        Number& self = dynamic_cast<Number&>( *( arguments.getReceiver() ) );
        Number& arg0 = dynamic_cast<Number&>( *( arguments.get(0) ) );

        return  self / arg0;

    }


    Object* arrayAt(PrimitiveArguments& arguments){
        Array& self = dynamic_cast<Array&>( *( arguments.getReceiver() ) );
        Number& arg0 = dynamic_cast<Number&>( *( arguments.get(0) ) );

        return self.at( arg0.truncate() );
    }

    Object* arrayPush(PrimitiveArguments& arguments){
        Array& self = dynamic_cast<Array&>( *( arguments.getReceiver() ) );

        return self.push( arguments.get(0) );
    }

    Object* arraySize(PrimitiveArguments& arguments){
        Array& self = dynamic_cast<Array&>( *( arguments.getReceiver() ) );

        return self.size();
    }

    Object* arrayTransient(PrimitiveArguments& arguments){
        auto self = dynamic_cast<Array&>( *( arguments.getReceiver() ) );

        return self.transient();
    }

    Object* arrayTransientPersist(PrimitiveArguments& arguments){
        auto self = dynamic_cast<ArrayTransient*>( arguments.getReceiver() );

        if (self == nullptr ) throw std::bad_cast();

        return self->persist();;

    }

    Object* arrayTransientPush(PrimitiveArguments& arguments){
        auto self = dynamic_cast<ArrayTransient*>( arguments.getReceiver() );

        if (self == nullptr ) throw std::bad_cast();

        return self->push( arguments.get(0) );
    }


    Object* mapAt(PrimitiveArguments& arguments){
        auto self = dynamic_cast<Map&>( *( arguments.getReceiver() ) );
        auto arg0 = dynamic_cast<String&>( *( arguments.get(0) ) );

        return self.at( arg0.getValue() );
    }

    Object* mapAtPut(PrimitiveArguments& arguments){
        auto self = dynamic_cast<Map&>( *( arguments.getReceiver() ) );
        auto index = dynamic_cast<String&>( *( arguments.get( 0 ) ) );

        return self.putAt( index.getValue(), arguments.get( 1 ) );
    }

    Object* mapTransient(PrimitiveArguments& arguments){
        auto self = dynamic_cast<Map&>( *( arguments.getReceiver() ) );

        auto t = self.transient();
        return t;
    }

    Object* mapTransientPersist(PrimitiveArguments& arguments){
        auto self = dynamic_cast<MapTransient*>( arguments.getReceiver() );

        if (self == nullptr ) throw std::bad_cast();

        return self->persist();;
    }

    Object* mapTransientAtPut(PrimitiveArguments& arguments){
        auto self = dynamic_cast<MapTransient*>( arguments.getReceiver() );
        auto index = dynamic_cast<String&>( *( arguments.get( 0 ) ) );

        if (self == nullptr ) throw std::bad_cast();

        self->putAt( index.getValue(), arguments.get( 1 ) );

        return arguments.getReceiver();
    }

    Object* arrayFormatString(PrimitiveArguments& arguments){
        auto self = dynamic_cast<Array&>( *( arguments.getReceiver() ) );
        auto arg0 = dynamic_cast<String&>( *( arguments.get(0) ) );

        return self.formatString( arg0 );

    }

    Object* methodEval(PrimitiveArguments& arguments){
        World& world = World::instance();
        Method& method = dynamic_cast<Method&>( *( arguments.getReceiver() ));
        // TODO check arity
        return world.vm.eval( method, nullptr );
    }

    Object* methodPrintByteCode(PrimitiveArguments& arguments){
        Method& method = dynamic_cast<Method&>( *( arguments.getReceiver() ));

        method.getCompiledMethod()->printBytecode();

        return &method;
    }

}
