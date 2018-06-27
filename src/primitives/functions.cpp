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

    Object* print(World*, Object* self, Object** args){

        std::cout << args[0]->toString();
        return self;
    }

    Object* endl(World*, Object* self, Object**){

        std::cout << std::endl;
        return self;
    }


    Object* equals(World* world, Object* self, Object** args){
        static auto _true = world->getTrue();
        static auto _false = world->getFalse();

        if ( *self == *( args[0] ) ){
            return _true;
        }else{
            return _false;
        }
    }


    Object* greater(World* world, Object* self, Object** args){
        static auto _true = world->getTrue();
        static auto _false = world->getFalse();

        if ( *self > *( args[0] ) ){
            return _true;
        }else{
            return _false;
        }
    }


    Object* less(World* world, Object* self, Object** args){
        static auto _true = world->getTrue();
        static auto _false = world->getFalse();

        if ( *self < *( args[0] ) ){
            return _true;
        }else{
            return _false;
        }
    }


    Object* greaterOrEqual(World* world, Object* self, Object** args){
        static auto _true = world->getTrue();
        static auto _false = world->getFalse();

        if ( *self >= *( args[0] ) ){
            return _true;
        }else{
            return _false;
        }
    }


    Object* lessOrEqual(World* world, Object* self, Object** args){
        static auto _true = world->getTrue();
        static auto _false = world->getFalse();

        if ( *self <= *( args[0] ) ){
            return _true;
        }else{
            return _false;
        }
    }


    Object* isIdenticalTo(World* world, Object* self, Object** args){
        static auto _true = world->getTrue();
        static auto _false = world->getFalse();

        if ( self == args[0] ){
            return _true;
        }else{
            return _false;
        }
    }

    Object* plus(World*, Object* self, Object** args){

        Number& _self = dynamic_cast<Number&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return  _self + arg0;

    }

    Object* minus(World*, Object* self, Object** args){

        Number& _self = dynamic_cast<Number&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return  _self - arg0;

    }

    Object* multiply(World*, Object* self, Object** args){

        Number& _self = dynamic_cast<Number&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return  _self * arg0;

    }

    Object* divide(World*, Object* self, Object** args){

        Number& _self = dynamic_cast<Number&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return  _self / arg0;

    }


    Object* sqrt(World*, Object* self, Object**){
        Number& _self = dynamic_cast<Number&>( *self );

        return _self.sqrt();
    }


    Object* random(World*, Object*, Object**){
        return Number::random();
    }

    Object* stringConcat(World*, Object* self, Object** args){

        String& _self = dynamic_cast<String&>( *self );
        String& arg0 = dynamic_cast<String&>( *( args[0] ) );

        return _self + arg0;

    }


    Object* arrayAt(World*, Object* self, Object** args){
        Array& _self = dynamic_cast<Array&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return _self.at( arg0.truncate() );
    }

    Object* arrayPush(World*, Object* self, Object** args){

        Array& _self = dynamic_cast<Array&>( *self );

        return _self.push( args[0] );
    }

    Object* arrayTake(World*, Object* self, Object** args){

        Array& _self = dynamic_cast<Array&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return _self.take( arg0.truncate() );
    }

    Object* arrayDrop(World*, Object* self, Object** args){
        Array& _self = dynamic_cast<Array&>( *self );
        Number& arg0 = dynamic_cast<Number&>( *( args[0] ) );

        return _self.drop( arg0.truncate() );
    }

    Object* arraySize(World*, Object* self, Object**){
        Array& _self = dynamic_cast<Array&>( *self );

        return _self.size();
    }

    Object* arrayTransient(World*, Object* self, Object**){
        auto _self = dynamic_cast<Array&>( *self );

        return _self.transient();
    }

    Object* arrayTransientPersist(World*, Object* self, Object**){
        auto _self = dynamic_cast<ArrayTransient*>( self );

        if (_self == nullptr ) throw std::bad_cast();

        return _self->persist();;

    }

    Object* arrayTransientPush(World*, Object* self, Object** args){
        auto _self = dynamic_cast<ArrayTransient*>( self );

        if (_self == nullptr ) throw std::bad_cast();

        return _self->push( args[0] );
    }


    Object* mapAt(World* world, Object* self, Object** args){
        auto _self = dynamic_cast<Map&>( *self );
        auto arg0 = dynamic_cast<String&>( *( args[0] ) );

        MapStringAdapter mapAdapter(world->constantsTable, _self);


        return mapAdapter.at( arg0.getValue() );
    }

    Object* mapAtPut(World* world, Object* self, Object** args){
        auto _self = dynamic_cast<Map&>( *self );
        auto index = dynamic_cast<String&>( *( args[0] ) );

        MapStringAdapter mapAdapter(world->constantsTable, _self);

        return mapAdapter.putAt( index.getValue(), args[1] );
    }

    Object* mapTransient(World*, Object* self, Object**){
        auto _self = dynamic_cast<Map&>( *self );

        auto t = _self.transient();
        return t;
    }

    Object* mapTransientPersist(World*, Object* self, Object**){
        auto _self = dynamic_cast<MapTransient*>( self );

        if (self == nullptr ) throw std::bad_cast();

        return _self->persist();;
    }

    Object* mapTransientAtPut(World* world, Object* self, Object** args){
        auto _self = dynamic_cast<MapTransient*>( self );
        auto index = dynamic_cast<String&>( *( args[0] ) );

        if (self == nullptr ) throw std::bad_cast();

        MapTransientStringAdapter mapAdapter(world->constantsTable, *_self);

        mapAdapter.putAt( index.getValue(), args[1] );

        return self;
    }

    Object* arrayFormatString(World*, Object* self, Object** args){
        auto _self = dynamic_cast<Array&>( *self );
        auto arg0 = dynamic_cast<String&>( *( args[0] ) );

        return _self.formatString( arg0.getValue() );

    }

    Object* methodEval(World* world, Object* self, Object**){
        Method& method = dynamic_cast<Method&>( *self );
        // TODO check arity
        return world->vm.eval( method );
    }

    Object* methodPrintByteCode(World*, Object* self, Object**){
        Method& method = dynamic_cast<Method&>( *self );

        method.getCompiledMethod()->printBytecode();

        return &method;
    }

    Object* loadPath(World* world, Object* self, Object** args){
        auto path = dynamic_cast<String&>( *( args[0] ) );

        world->loadPackage( path.toString() );

        return self;
    }

    Object* loadNative(World* world, Object* self, Object** args){
        auto path = dynamic_cast<String&>( *( args[0] ) );

        world->loadNative( path.toString() );

        return self;
    }


}
