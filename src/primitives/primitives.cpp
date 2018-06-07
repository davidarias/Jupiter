// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <primitives/primitives.hpp>

#include <objects/Objects.hpp>
#include <vm/World.hpp>

namespace jupiter{

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


    Object* sqrt(PrimitiveArguments& arguments){
        Number& self = dynamic_cast<Number&>( *( arguments.getReceiver() ) );

        return self.sqrt();
    }


    Object* random(PrimitiveArguments& arguments){
        return Number::random();
    }

    Object* stringConcat(PrimitiveArguments& arguments){
        String& self = dynamic_cast<String&>( *( arguments.getReceiver() ) );
        String& arg0 = dynamic_cast<String&>( *( arguments.get(0) ) );

        return self + arg0;

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

    Object* arrayTake(PrimitiveArguments& arguments){
        Array& self = dynamic_cast<Array&>( *( arguments.getReceiver() ) );
        Number& arg0 = dynamic_cast<Number&>( *( arguments.get(0) ) );

        return self.take( arg0.truncate() );
    }

    Object* arrayDrop(PrimitiveArguments& arguments){
        Array& self = dynamic_cast<Array&>( *( arguments.getReceiver() ) );
        Number& arg0 = dynamic_cast<Number&>( *( arguments.get(0) ) );

        return self.drop( arg0.truncate() );
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

        return self.formatString( arg0.getValue() );

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

    Object* loadPath(PrimitiveArguments& arguments){
        auto path = dynamic_cast<String&>( *( arguments.get(0) ) );
        World::instance().loadPackage( path.toString() );

        return World::instance().getNil();
    }

    Object* loadNative(PrimitiveArguments& arguments){
        auto path = dynamic_cast<String&>( *( arguments.get(0) ) );

        World::instance().loadNative( path.toString() );

        return World::instance().getNil();
    }

    Primitives::Primitives(){
        primitives.reserve(22);

        // common
        add("print", 1, print);
        add("endl", 0, endl );

        add("isIdenticalTo", 1, isIdenticalTo);
        add("equals",          1, equals ) ;
        add("greater",         1, greater ) ;
        add("greaterOrEqual",  1, greaterOrEqual ) ;
        add("less",            1, less ) ;
        add("lessOrEqual",     1, lessOrEqual ) ;

        // numbers
        add("plus",      1, plus ) ;
        add("minus",     1, minus ) ;
        add("multiply",  1, multiply ) ;
        add("divide",    1, divide ) ;
        add("sqrt",      0, sqrt ) ;

        add("random",     0, random ) ;

        // strings
        add("stringConcat", 1, stringConcat ) ;

        // arrays
        add("arrayAt",            1, arrayAt ) ;
        add("arrayPush",          1, arrayPush ) ;
        add("arrayTake",          1, arrayTake ) ;
        add("arrayDrop",          1, arrayDrop ) ;
        add("arraySize",          0, arraySize ) ;
        add("arrayFormatString",  1, arrayFormatString ) ;

        add("arrayTransient",        0, arrayTransient ) ;
        add("arrayTransientPersist", 0, arrayTransientPersist ) ;
        add("arrayTransientPush",    1, arrayTransientPush ) ;

        // maps
        add("mapAt",               1, mapAt ) ;
        add("mapAtPut",            2, mapAtPut ) ;
        add("mapTransient",        0, mapTransient );
        add("mapTransientPersist", 0, mapTransientPersist ) ;
        add("mapTransientAtPut",   2, mapTransientAtPut ) ;

        // methods
        add("eval0",  0, methodEval );
        add("eval1",  1, methodEval );
        add("eval2",  2, methodEval );
        add("eval3",  3, methodEval );
        add("printBytecode", 0, methodPrintByteCode );


        add("loadPath", 1, loadPath );
        add("loadNative", 1, loadNative );
    }

    Primitives::~Primitives(){}

    void Primitives::add(std::string name, unsigned arity, PrimitiveFunction primitiveFunction){
        PrimitiveMethod primitive(primitiveFunction, arity);
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


}
