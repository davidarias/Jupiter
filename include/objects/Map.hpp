// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __MAP_H
#define __MAP_H

#include <immer/map.hpp>

#include <objects/Object.hpp>


namespace jupiter{


    class Map: public Object{
    private:
        immer::map<unsigned, Object* > slots;
    protected:

        int cmp(Object&);
        bool equal(Object& other);

    public:

        Map();
        Map(Map& other);
        Map(immer::map<unsigned, Object* > slots);

        void accept(ObjectVisitor&);

        void mark();

        std::string toString();

        Object* at(const unsigned key);
        Object* putAt(const unsigned key, Object* value);
        void putAtMut(const unsigned key, Object* value);

        Object* transient();
    };

    class ConstantsTable;

    // Map class uses unsigned int as keys, but usually we need to
    // use strings keys, so this adapter helps with that using
    // the table of interned strings ( for now the ConstantsTable class )
    class MapStringAdapter{
    private:
        ConstantsTable& table;
        Map& map;
    public:
        MapStringAdapter(ConstantsTable& table, Map& map);
        Object* at(const std::string& key);
        Object* putAt(const std::string& key, Object* value);
        void putAtMut(const std::string& key, Object* value);
    };

    class MapTransient : public Object{
    private:
        immer::map<unsigned, Object* > slots;
    protected:
        int cmp(Object&);
    public:
        MapTransient();
        MapTransient(immer::map<unsigned, Object* > slots);
        void putAt(const unsigned key, Object* value);
        Object* persist();

        void mark();
        void accept(ObjectVisitor&);
        std::string toString();
    };
    
    
    
    class MapTransientStringAdapter{
    private:
        ConstantsTable& table;
        MapTransient& map;
    public:
        MapTransientStringAdapter(ConstantsTable& table, MapTransient& map);
        void putAt(const std::string& key, Object* value);
    };

}

#endif
