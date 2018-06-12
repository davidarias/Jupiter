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
    protected:

        int cmp(Object&);
        bool equal(Object& other);
    public:
        immer::map<std::string, Object* > slots;

        Map();
        Map(immer::map<std::string, Object* > slots);

        void mark();

        std::string toString();
        void eval(Evaluator& evaluator);

        Object* at(const std::string& key);
        Object* putAt(const std::string& key, Object* value);
        void putAtMut(const std::string& key, Object* value);

        Object* transient();
    };

    class MapTransient : public Map{
    public:
        MapTransient();
        MapTransient(immer::map<std::string, Object* > slots);
        Object* at(const std::string& key);
        void putAt(const std::string& key, Object* value);
        Object* persist();
    };

}

#endif
