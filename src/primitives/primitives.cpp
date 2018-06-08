// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <primitives/primitives.hpp>

#include <objects/Objects.hpp>
#include <vm/World.hpp>

namespace jupiter{

    Object* print(Object* self, Object** args){

        std::cout << args[0]->toString();
        return self;
    }

    Object* endl(Object* self, Object** args){

        std::cout << std::endl;
        return self;
    }


    Object* equals(Object* self, Object** args){

        if ( *self == *( args[0] ) ){
            return World::instance().getTrue();
        }else{
            return World::instance().getFalse();
        }
    }


    Object* greater(Object* self, Object** args){

        if ( *self > *( args[0] ) ){
            return World::instance().getTrue();
        }else{
            return World::instance().getFalse();
        }
    }


    Object* less(Object* self, Object** args){

        if ( *self < *( args[0] ) ){
            return World::instance().getTrue();
        }else{
            return World::instance().getFalse();
        }
    }


    Object* greaterOrEqual(Object* self, Object** args){

        if ( *self >= *( args[0] ) ){
            return World::instance().getTrue();
        }else{
            return World::instance().getFalse();
        }
    }


    Object* lessOrEqual(Object* self, Object** args){

        if ( *self <= *( args[0] ) ){
            return World::instance().getTrue();
        }else{
            return World::instance().getFalse();
        }
    }


    Object* isIdenticalTo(Object* self, Object** args){

        if ( self == args[0] ){
            return World::instance().getTrue();
        }else{
            return World::instance().getFalse();
        }
    }

    Object* plus(Object* self, Object** args){

        Number& _self = dynamic_cast<Number&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return  _self + arg0;

    }

    Object* minus(Object* self, Object** args){

        Number& _self = dynamic_cast<Number&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return  _self - arg0;

    }

    Object* multiply(Object* self, Object** args){

        Number& _self = dynamic_cast<Number&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return  _self * arg0;

    }

    Object* divide(Object* self, Object** args){

        Number& _self = dynamic_cast<Number&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return  _self / arg0;

    }


    Object* sqrt(Object* self, Object** args){
        Number& _self = dynamic_cast<Number&>( *self );

        return _self.sqrt();
    }


    Object* random(Object* self, Object** args){
        return Number::random();
    }

    Object* stringConcat(Object* self, Object** args){

        String& _self = dynamic_cast<String&>( *self );
        String& arg0 = dynamic_cast<String&>( *( args[0] ) );

        return _self + arg0;

    }


    Object* arrayAt(Object* self, Object** args){
        Array& _self = dynamic_cast<Array&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return _self.at( arg0.truncate() );
    }

    Object* arrayPush(Object* self, Object** args){

        Array& _self = dynamic_cast<Array&>( *self );

        return _self.push( args[0] );
    }

    Object* arrayTake(Object* self, Object** args){

        Array& _self = dynamic_cast<Array&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return _self.take( arg0.truncate() );
    }

    Object* arrayDrop(Object* self, Object** args){
        Array& _self = dynamic_cast<Array&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return _self.drop( arg0.truncate() );
    }

    Object* arraySize(Object* self, Object** args){
        Array& _self = dynamic_cast<Array&>( *self );

        return _self.size();
    }

    Object* arrayTransient(Object* self, Object** args){
        auto _self = dynamic_cast<Array&>( *self );

        return _self.transient();
    }

    Object* arrayTransientPersist(Object* self, Object** args){
        auto _self = dynamic_cast<ArrayTransient*>( self );

        if (_self == nullptr ) throw std::bad_cast();

        return _self->persist();;

    }

    Object* arrayTransientPush(Object* self, Object** args){
        auto _self = dynamic_cast<ArrayTransient*>( self );

        if (_self == nullptr ) throw std::bad_cast();

        return _self->push( args[0] );
    }


    Object* mapAt(Object* self, Object** args){
        auto _self = dynamic_cast<Map&>( *self );
        auto arg0 = dynamic_cast<String&>( *( args[0] ) );

        return _self.at( arg0.getValue() );
    }

    Object* mapAtPut(Object* self, Object** args){
        auto _self = dynamic_cast<Map&>( *self );
        auto index = dynamic_cast<String&>( *( args[0] ) );

        return _self.putAt( index.getValue(), args[1] );
    }

    Object* mapTransient(Object* self, Object** args){
        auto _self = dynamic_cast<Map&>( *self );

        auto t = _self.transient();
        return t;
    }

    Object* mapTransientPersist(Object* self, Object** args){
        auto _self = dynamic_cast<MapTransient*>( self );

        if (self == nullptr ) throw std::bad_cast();

        return _self->persist();;
    }

    Object* mapTransientAtPut(Object* self, Object** args){
        auto _self = dynamic_cast<MapTransient*>( self );
        auto index = dynamic_cast<String&>( *( args[0] ) );

        if (self == nullptr ) throw std::bad_cast();

        _self->putAt( index.getValue(), args[1] );

        return self;
    }

    Object* arrayFormatString(Object* self, Object** args){
        auto _self = dynamic_cast<Array&>( *self );
        auto arg0 = dynamic_cast<String&>( *( args[0] ) );

        return _self.formatString( arg0.getValue() );

    }

    Object* methodEval(Object* self, Object** args){
        World& world = World::instance();
        Method& method = dynamic_cast<Method&>( *self );
        // TODO check arity
        return world.vm.eval( method, nullptr );
    }

    Object* methodPrintByteCode(Object* self, Object** args){
        Method& method = dynamic_cast<Method&>( *self );

        method.getCompiledMethod()->printBytecode();

        return &method;
    }

    Object* loadPath(Object* self, Object** args){
        auto path = dynamic_cast<String&>( *( args[0] ) );
        World::instance().loadPackage( path.toString() );

        return World::instance().getNil();
    }

    Object* loadNative(Object* self, Object** args){
        auto path = dynamic_cast<String&>( *( args[0] ) );

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

    void Primitives::add(std::string name, unsigned arity, NativeFunction primitiveFunction){
        NativeMethod primitive(primitiveFunction, arity);
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
