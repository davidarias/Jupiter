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

        Object* at(const std::string& key);
        Object* at(const unsigned key);
        Object* putAt(const std::string& key, Object* value);
        void putAtMut(const std::string& key, Object* value);

        Object* transient();
    };

    class MapTransient : public Object{
    private:
        immer::map<unsigned, Object* > slots;
    protected:
        int cmp(Object&);
    public:
        MapTransient();
        MapTransient(immer::map<unsigned, Object* > slots);
        void putAt(const std::string& key, Object* value);
        Object* persist();

        void mark();
        void accept(ObjectVisitor&);
        std::string toString();
    };

}

#endif
