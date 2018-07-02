// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.


#ifndef __FUNCTIONS_H_
#define __FUNCTIONS_H_

namespace jupiter{

    class Object;
    class World;

    Object* print(World* world, Object* self, Object** args);
    Object* endl(World* world, Object* self, Object** args);


    Object* equals(World* world, Object* self, Object** args);
    Object* greater(World* world, Object* self, Object** args);
    Object* less(World* world, Object* self, Object** args);
    Object* greaterOrEqual(World* world, Object* self, Object** args);
    Object* lessOrEqual(World* world, Object* self, Object** args);
    Object* isIdenticalTo(World* world, Object* self, Object** args);


    Object* plus(World* world, Object* self, Object** args);
    Object* minus(World* world, Object* self, Object** args);
    Object* multiply(World* world, Object* self, Object** args);
    Object* divide(World* world, Object* self, Object** args);
    Object* sqrt(World* world, Object* self, Object** args);
    Object* random(World* world, Object* self, Object** args);

    Object* stringConcat(World* world, Object* self, Object** args);

    Object* arrayFormatString(World* world, Object* self, Object** args);
    Object* arrayAt(World* world, Object* self, Object** args);
    Object* arrayPush(World* world, Object* self, Object** args);
    Object* arrayTake(World* world, Object* self, Object** args);
    Object* arrayDrop(World* world, Object* self, Object** args);
    Object* arraySize(World* world, Object* self, Object** args);
    Object* arrayTransient(World* world, Object* self, Object** args);
    Object* arrayTransientPersist(World* world, Object* self, Object** args);
    Object* arrayTransientPush(World* world, Object* self, Object** args);

    Object* mapAt(World* world, Object* self, Object** args);
    Object* mapAtPut(World* world, Object* self, Object** args);
    Object* mapTransient(World* world, Object* self, Object** args);
    Object* mapTransientPersist(World* world, Object* self, Object** args);
    Object* mapTransientAtPut(World* world, Object* self, Object** args);

    Object* methodEval(World* world, Object* self, Object** args);
    Object* methodPrintByteCode(World* world, Object* self, Object** args);

    Object* loadPath(World* world, Object* self, Object** args);
    Object* loadNative(World* world, Object* self, Object** args);
    Object* evalString(World* world, Object* self, Object** args);
}

#endif
