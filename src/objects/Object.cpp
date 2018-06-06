// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <objects/Objects.hpp>

#include <vm/MemoryManager.hpp>
#include <vm/World.hpp>

#include <misc/Exceptions.hpp>

#include <utils/format.hpp>

namespace jupiter{

    void GCObject::mark(){
        marked = true;
    }

    void GCObject::unmark(){
        marked = false;
    }

    bool GCObject::isMarked(){
        return marked;
    }

    bool GCObject::istenured(){
        return tenured;
    }

    void GCObject::setTenured(){
        tenured = true;
    }

    Object::Object(){}
    Object::~Object(){}

    bool Object::equal(Object& other){
        return this->cmp(other) == 0;
    }

    bool operator==(Object& a, Object& b){
        if ( typeid( a ) == typeid( b ) && a.equal( b ) ){
            return true;
        }
        return false;
    }

    bool operator!=(Object& a, Object& b){
        return !( a == b );
    }

    bool operator>(Object& a, Object& b){
        if ( typeid( a ) != typeid( b ) ) throw "Diferent types are not comparable";
        if (  a.cmp( b ) > 0 ){
            return true;
        }
        return false;
    }
    bool operator<(Object& a, Object& b){
        if ( typeid( a ) != typeid( b ) ) throw "Diferent types are not comparable";
        if ( a.cmp( b ) < 0 ){
            return true;
        }
        return false;
    }

    bool operator<=(Object& a, Object& b){
        if ( typeid( a ) != typeid( b ) ) throw "Diferent types are not comparable";
        if ( a.cmp(b) < 0 || a.equal(b) ) return true;
        return false;
    }

    bool operator>=(Object& a, Object& b){
        if ( typeid( a ) != typeid( b ) ) throw "Diferent types are not comparable";
        if ( a.cmp(b) > 0 || a.equal(b) ) return true;
        return false;
    }

    Map::Map(){};
    Map::Map(immer::map<std::string, Object* > slots) : slots(slots) {}

    void Map::mark(){
        marked = true;
        for(auto& kv : slots){
            (*kv.second).mark();
        }
    }

    int Map::cmp(Object&){
        throw "Object Maps cannot be compared";
    }

    bool Map::equal(Object& other){
        auto otherMap = static_cast<Map&>(other);

        if ( slots.size() != otherMap.slots.size() ) return false;

        for(auto& kv : slots){
            auto o = otherMap.slots.find( kv.first );
            if (!o) return false;
            Object& value = *(*o);
            if ( value != (*kv.second) ) return false;
        }

        return true;
    }

    Object* Map::at(const std::string& selector){
        auto o = slots.find( selector );
        if ( o ) return *o;
        // globals are always maps
        static Map& mapBehaviour = static_cast<Map&>( *( World::instance().globals.at("Map") ));
        try{
            return mapBehaviour.slots.at( selector );
        }catch(std::exception& e){
            throw  "Selector \'" + selector + "\' not found in " + this->toString();
        }
    }

    std::string Map::toString(){
        auto o = slots.find( "toString" );
        if ( o ){
            auto result = World::instance().eval(*o, this);
            return result->toString();
        }
        std::ostringstream buffer;
        buffer << "Object " << this;
        return buffer.str();
    }

    void Map::eval(Evaluator& evaluator){
        evaluator(*this);
    }

    Object* Map::putAt(const std::string& key, Object* value){
        return MemoryManager<Map>::instance().get( slots.set( key, value ) );
    }

    void Map::putAtMut(const std::string& key, Object* value){
        slots = std::move(slots).set( key, value );
    }

    Object* Map::transient(){
        return MemoryManager<MapTransient>::instance().get( slots );
    }

    MapTransient::MapTransient() {}
    MapTransient::MapTransient(immer::map<std::string, Object* > slots) : Map(slots) {}

    Object* MapTransient::at(const std::string& selector){

        // globals are always maps
        static Map& behaviour = static_cast<Map&>( *( World::instance().globals.at("MapTransient") ));

        return behaviour.at( selector );

    }

    void MapTransient::putAt(const std::string& key, Object* value){
        // transients can point to young objects
        // being tenured, so when adding an object
        // to a tenured transient we also mark
        // this object as tenured. when minor gc cycle
        // is executed, we move this object to tenured space
        if ( istenured() ){
            value->setTenured();
        }
        slots = std::move(slots).set( key, value );
    }

    Object* MapTransient::persist(){
        return MemoryManager<Map>::instance().get( slots );
    }

    Array::Array(){}
    Array::Array( immer::flex_vector<Object*> values ) : values( values ){}

    Array::Array(std::vector<Object*>::iterator start, std::vector<Object*>::iterator end)
        : values( start, end ) {}

    void Array::mark(){
        marked = true;
        for(auto v : values){
            v->mark();
        }
    }

    Object* Array::at(const std::string& selector){
        // globals are always maps
        static Map& behaviour =  static_cast<Map&>( *( World::instance().globals.at("Array") ));
        return behaviour.at( selector );
    }

    Object* Array::push( Object* value ){
        return MemoryManager<Array>::instance().get( values.push_back(value) );
    }

    Object* Array::take( int elems ){
        return MemoryManager<Array>::instance().get( values.take( elems ) );
    }

    Object* Array::drop( int elems ){
        return MemoryManager<Array>::instance().get( values.drop( elems ) );
    }

    Object* Array::size(){
        return MemoryManager<Number>::instance().get( values.size() );
    }

    Object* Array::formatString(std::string& str){
        return MemoryManager<String>::instance().get( format( str, values ) );
    }

    Object* Array::at( int index ){
        return values.at( index - 1 ); // arrays starts at index 1
    }

    bool Array::equal(Object& other){
        // we checked the type in the == operator
        auto otherArray = static_cast<Array&>(other);
        if ( values.size() != otherArray.values.size() ) return false;

        auto it1 = values.begin();
        auto it1End = values.end();

        auto it2 = otherArray.values.begin();
        auto it2End = otherArray.values.end();

        while( it1 != it1End && it2 != it2End ){
            Object& a = *(*it1);
            Object& b = *(*it2);

            if ( a == b ){
                ++it1; ++it2;
                continue;
            }else{
                return false;
            }
        }

        return true;
    }

    int Array::cmp(Object& other){
        // we checked the type in the == operator
        auto otherArray = static_cast<Array&>(other);

        auto it1 = values.begin();
        auto it1End = values.end();

        auto it2 = otherArray.values.begin();
        auto it2End = otherArray.values.end();

        while( it1 != it1End && it2 != it2End ){
            Object& a = *(*it1);
            Object& b = *(*it2);

            if ( a < b ){
                return -1;
            }else if( a > b){
                return 1;
            }
            ++it1; ++it2;
        }

        // for now all values are the same, check the size to return
        if ( values.size() == otherArray.values.size() ){
            return 0;
        }else if ( values.size() < otherArray.values.size() ){
            return -1;
        }else{
            return 1;
        }
    }

    std::string Array::toString(){
        std::ostringstream buffer;
        buffer << "Array " << this;
        return buffer.str();
    }



    void Array::eval(Evaluator& evaluator){
        evaluator(*this);
    }

    Object* Array::transient(){
        return MemoryManager<ArrayTransient>::instance().get( values );
    }

    ArrayTransient::ArrayTransient() {}
    ArrayTransient::ArrayTransient(immer::flex_vector<Object*> values) : Array( values ) {}

    Object* ArrayTransient::at(const std::string& selector){

        static Map& behaviour = static_cast<Map&>( *( World::instance().globals.at("ArrayTransient") ));
        return behaviour.at( selector );

    }

    Object* ArrayTransient::push( Object* value){
        // transients can point to young objects
        // being tenured, so when adding an object
        // to a tenured transient we also mark
        // this object as tenured. when minor gc cycle
        // is executed, we move this object to tenured space
        if ( istenured() ){
            value->setTenured();
        }
        values = std::move(values).push_back( value );
        return this;
    }

    Object* ArrayTransient::persist(){
        return MemoryManager<Array>::instance().get( values );
    }

    Method::Method(){}

    Method::Method(std::string& name, std::string& signature, std::string& source,
                   std::shared_ptr<CompiledMethod> compiledMethod)
        : name(name), signature(signature), source(source), compiledMethod(compiledMethod), self(nullptr) {}

    Method::Method(std::shared_ptr<CompiledMethod> compiledMethod)
        : compiledMethod(compiledMethod), self(nullptr){}

    Method::~Method(){}

    void Method::mark(){
        marked = true;

        if ( self != nullptr ) self->mark();

        for(auto& pair : upvalues){
            auto upvalue = pair.second;
            upvalue->mark();

        }
    }

    std::shared_ptr<CompiledMethod> Method::getCompiledMethod(){
        return compiledMethod;
    }


    bool Method::equal(Object& other){
        if (&other == this) return true;
        return false;
    }

    int Method::cmp(Object&){
        throw "Methods cannot be compared";
    }

    std::string& Method::getName(){
        return name;
    }

    Object* Method::at(const std::string& selector){
        // globals are always maps
        static Map& behaviour =  static_cast<Map&>( *( World::instance().globals.at("Method") ));
        return behaviour.at( selector );
    }

    std::string Method::toString(){
        std::ostringstream buffer;
        buffer << "Method " << this;
        return buffer.str();
    }

    void Method::eval(Evaluator& evaluator){
        evaluator(*this);
    }

    PrimitiveMethod::PrimitiveMethod(PrimitiveFunction fn, unsigned arity) :
        fn(fn), arity(arity) {}

    bool PrimitiveMethod::equal(Object& other){
        if (&other == this) return true;
        return false;
    }
    int PrimitiveMethod::cmp(Object&){
        throw "Primitive methods cannot be compared";
    }

    Object* PrimitiveMethod::at(const std::string& selector){
        throw  "Selector \'" + selector + "\' not found in " + this->toString();
    }

    std::string PrimitiveMethod::toString(){
        std::stringstream buffer;
        buffer << "Primitive Method " << this;
        return buffer.str();
    }

    void PrimitiveMethod::eval(Evaluator& evaluator){
        evaluator(*this);
    }


}
