// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <primitives/functions.hpp>

#include <objects/Objects.hpp>
#include <objects/CompiledMethod.hpp>
#include <vm/World.hpp>
#include <vm/ConstantsTable.hpp>

namespace jupiter{

    Object* print(Object* self, Object** args){

        std::cout << args[0]->toString();
        return self;
    }

    Object* endl(Object* self, Object**){

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


    Object* sqrt(Object* self, Object**){
        Number& _self = dynamic_cast<Number&>( *self );

        return _self.sqrt();
    }


    Object* random(Object*, Object**){
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

    Object* arraySize(Object* self, Object**){
        Array& _self = dynamic_cast<Array&>( *self );

        return _self.size();
    }

    Object* arrayTransient(Object* self, Object**){
        auto _self = dynamic_cast<Array&>( *self );

        return _self.transient();
    }

    Object* arrayTransientPersist(Object* self, Object**){
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

        MapStringAdapter mapAdapter(ConstantsTable::instance(), _self);


        return mapAdapter.at( arg0.getValue() );
    }

    Object* mapAtPut(Object* self, Object** args){
        auto _self = dynamic_cast<Map&>( *self );
        auto index = dynamic_cast<String&>( *( args[0] ) );

        MapStringAdapter mapAdapter(ConstantsTable::instance(), _self);

        return mapAdapter.putAt( index.getValue(), args[1] );
    }

    Object* mapTransient(Object* self, Object**){
        auto _self = dynamic_cast<Map&>( *self );

        auto t = _self.transient();
        return t;
    }

    Object* mapTransientPersist(Object* self, Object**){
        auto _self = dynamic_cast<MapTransient*>( self );

        if (self == nullptr ) throw std::bad_cast();

        return _self->persist();;
    }

    Object* mapTransientAtPut(Object* self, Object** args){
        auto _self = dynamic_cast<MapTransient*>( self );
        auto index = dynamic_cast<String&>( *( args[0] ) );

        if (self == nullptr ) throw std::bad_cast();

        MapTransientStringAdapter mapAdapter(ConstantsTable::instance(), *_self);

        mapAdapter.putAt( index.getValue(), args[1] );

        return self;
    }

    Object* arrayFormatString(Object* self, Object** args){
        auto _self = dynamic_cast<Array&>( *self );
        auto arg0 = dynamic_cast<String&>( *( args[0] ) );

        return _self.formatString( arg0.getValue() );

    }

    Object* methodEval(Object* self, Object**){
        World& world = World::instance();
        Method& method = dynamic_cast<Method&>( *self );
        // TODO check arity
        return world.vm.eval( method );
    }

    Object* methodPrintByteCode(Object* self, Object**){
        Method& method = dynamic_cast<Method&>( *self );

        method.getCompiledMethod()->printBytecode();

        return &method;
    }

    Object* loadPath(Object*, Object** args){
        auto path = dynamic_cast<String&>( *( args[0] ) );
        World::instance().loadPackage( path.toString() );

        return World::instance().getNil();
    }

    Object* loadNative(Object*, Object** args){
        auto path = dynamic_cast<String&>( *( args[0] ) );

        World::instance().loadNative( path.toString() );

        return World::instance().getNil();
    }


}
