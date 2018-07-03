// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <objects/Map.hpp>
#include <memory/memory.hpp>
#include <misc/Exceptions.hpp>
#include <vm/ConstantsTable.hpp>

namespace jupiter{

    Map::Map(){};
    Map::Map(Map& other) : slots( other.slots ){};
    Map::Map(immer::map<unsigned, Object* > slots) : slots(slots) {};

    void Map::mark(){
        marked = true;
        for(auto& kv : slots){
            (*kv.second).mark();
        }
    }

    int Map::cmp(Object&){
        throw RuntimeException("Object Maps cannot be compared");
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


    Object* Map::at(const unsigned selector){
        try{
            return slots.at( selector );
        }catch(std::exception& e){
            throw SelectorNotFound(selector);
        }
    }

    std::string Map::toString(){
        std::ostringstream buffer;
        buffer << "Object " << this;
        return buffer.str();
    }

    void Map::accept(ObjectVisitor& visitor){
        visitor.visit(*this);
    }

    Object* Map::putAt(const unsigned key, Object* value){
        return make<Map>( slots.set( key, value ) );
    }

    void Map::putAtMut(const unsigned key, Object* value){
        slots = std::move(slots).set(key, value );
    }

    Object* Map::transient(){
        return make<MapTransient>( slots );
    }

    MapStringAdapter::MapStringAdapter(ConstantsTable& table, Map& map): table(table), map(map){}

    Object* MapStringAdapter::at(const std::string& key){
        return map.at( table.string( key ) );
    }

    Object* MapStringAdapter::putAt(const std::string& key, Object* value){
        return map.putAt( table.string( key ), value );
    }

    void MapStringAdapter::putAtMut(const std::string& key, Object* value){
        map.putAtMut( table.string( key ), value );
    }

    MapTransient::MapTransient() {}
    MapTransient::MapTransient(immer::map<unsigned, Object* > slots) : slots(slots) {}

    void MapTransient::putAt(const unsigned key, Object* value){
        // transients can point to younger objects
        // breaking the invariant of innmutable containers
        // ( adding objects will make new containers)
        // to avoid invalid collection of elements
        // on minor cycles (since this object can be tenured
        // its elements won't be marked )
        // we mark all objects added as a precaution
        // making them tenured
        value->mark();
        slots = std::move(slots).set( key, value );
    }

    Object* MapTransient::persist(){
        return make<Map>( slots );
    }

    int MapTransient::cmp(Object&){
        throw RuntimeException("Object Maps cannot be compared");
    }

    std::string MapTransient::toString(){
        std::ostringstream buffer;
        buffer << "Map Transient " << this;
        return buffer.str();
    }

    void MapTransient::accept(ObjectVisitor& visitor){
        visitor.visit(*this);
    }

    void MapTransient::mark(){
        marked = true;
        for(auto& kv : slots){
            (*kv.second).mark();
        }
    }

    MapTransientStringAdapter::MapTransientStringAdapter(ConstantsTable& table, MapTransient& map): table(table), map(map){}

    void MapTransientStringAdapter::putAt(const std::string& key, Object* value){
        map.putAt( table.string( key ), value );
    }

}
