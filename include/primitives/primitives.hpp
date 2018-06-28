// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __PRIMITIVES_H
#define __PRIMITIVES_H

#include <misc/common.hpp>

#include <objects/Objects.hpp>

#include <objects/CompiledMethod.hpp>

namespace jupiter{

    class Primitives{

    private:

        std::vector<NativeMethod> primitives;
        std::unordered_map<std::string, unsigned> primitivesMap;

    public:
        Primitives();

        void add(std::string name, unsigned arity, NativeFunction primitiveFunction);
        Object* get(std::string& name);


    };

}
#endif
