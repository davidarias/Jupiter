// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __PRIMITIVES_H
#define __PRIMITIVES_H

#include <misc/common.hpp>

#include <objects/Object.hpp>
#include <objects/Number.hpp>
#include <objects/CompiledMethod.hpp>
#include <primitives/PrimitiveArguments.hpp>

#include <vm/World.hpp>

namespace jupiter{

    class Primitives{

    private:
        Primitives();
        ~Primitives();
        Primitives(const Primitives& ) = delete;
        void operator=(const Primitives& ) = delete;

        std::vector<PrimitiveMethod> primitives;
        std::unordered_map<std::string, unsigned> primitivesMap;

    public:

        static Primitives& instance() {
            static Primitives i;
            return i;
        }

        void add(std::string name, PrimitiveMethod primitive);
        Object* get(std::string& name);


    };

    Object* print(PrimitiveArguments& arguments);
    Object* endl(PrimitiveArguments& arguments);

    Object* equals(PrimitiveArguments& arguments);
    Object* greater(PrimitiveArguments& arguments);
    Object* less(PrimitiveArguments& arguments);
    Object* greaterOrEqual(PrimitiveArguments& arguments);
    Object* lessOrEqual(PrimitiveArguments& arguments);
    Object* isIdenticalTo(PrimitiveArguments& arguments);


    Object* plus(PrimitiveArguments& arguments);
    Object* minus(PrimitiveArguments& arguments);
    Object* multiply(PrimitiveArguments& arguments);
    Object* divide(PrimitiveArguments& arguments);

    Object* mapAt(PrimitiveArguments& arguments);
    Object* mapAtPut(PrimitiveArguments& arguments);

    Object* mapTransient(PrimitiveArguments& arguments);
    Object* mapTransientPersist(PrimitiveArguments& arguments);
    Object* mapTransientAtPut(PrimitiveArguments& arguments);


    Object* arrayAt(PrimitiveArguments& arguments);
    Object* arrayPush(PrimitiveArguments& arguments);
    Object* arraySize(PrimitiveArguments& arguments);
    Object* arrayFormatString(PrimitiveArguments& arguments);

    Object* arrayTransient(PrimitiveArguments& arguments);
    Object* arrayTransientPersist(PrimitiveArguments& arguments);
    Object* arrayTransientPush(PrimitiveArguments& arguments);

    Object* methodEval(PrimitiveArguments& arguments);
    Object* methodPrintByteCode(PrimitiveArguments& arguments);


}
#endif
