// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <objects/Map.hpp>
#include <vm/MemoryManager.hpp>

namespace jupiter{

    Map::Map(){};
    Map::Map(Map& other) : slots( other.slots ){};
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
        static Map& mapBehaviour = static_cast<Map&>( *( World::instance().prototypes.at("Map") ));
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
        static Map& behaviour = static_cast<Map&>( *( World::instance().prototypes.at("MapTransient") ));

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

}
