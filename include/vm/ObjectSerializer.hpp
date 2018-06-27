// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __OBJECTSERIALIZER_H
#define __OBJECTSERIALIZER_H

#include <misc/common.hpp>

namespace jupiter{

    class Map;

    class World;

    class ObjectSerializer{
    private:
        World& world;

    public:
        ObjectSerializer(World& world);
        void deserialize(std::string path, Map* root);

    private:
        void deserializeMethod(std::string path, Map* obj);

    };
}
#endif
