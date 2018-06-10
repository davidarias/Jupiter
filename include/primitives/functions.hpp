// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.


#ifndef __FUNCTIONS_H_
#define __FUNCTIONS_H_

namespace jupiter{

    class Object;


    Object* print(Object* self, Object** args);
    Object* endl(Object* self, Object** args);


    Object* equals(Object* self, Object** args);
    Object* greater(Object* self, Object** args);
    Object* less(Object* self, Object** args);
    Object* greaterOrEqual(Object* self, Object** args);
    Object* lessOrEqual(Object* self, Object** args);
    Object* isIdenticalTo(Object* self, Object** args);


    Object* plus(Object* self, Object** args);
    Object* minus(Object* self, Object** args);
    Object* multiply(Object* self, Object** args);
    Object* divide(Object* self, Object** args);
    Object* sqrt(Object* self, Object** args);
    Object* random(Object* self, Object** args);


    Object* stringConcat(Object* self, Object** args);


    Object* arrayFormatString(Object* self, Object** args);
    Object* arrayAt(Object* self, Object** args);
    Object* arrayPush(Object* self, Object** args);
    Object* arrayTake(Object* self, Object** args);
    Object* arrayDrop(Object* self, Object** args);
    Object* arraySize(Object* self, Object** args);
    Object* arrayTransient(Object* self, Object** args);
    Object* arrayTransientPersist(Object* self, Object** args);
    Object* arrayTransientPush(Object* self, Object** args);

    Object* mapAt(Object* self, Object** args);
    Object* mapAtPut(Object* self, Object** args);
    Object* mapTransient(Object* self, Object** args);
    Object* mapTransientPersist(Object* self, Object** args);
    Object* mapTransientAtPut(Object* self, Object** args);

    Object* methodEval(Object* self, Object** args);
    Object* methodPrintByteCode(Object* self, Object** args);

    Object* loadPath(Object* self, Object** args);
    Object* loadNative(Object* self, Object** args);
}

#endif
