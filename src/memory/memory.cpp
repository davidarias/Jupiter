// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <memory/memory.hpp>

namespace jupiter{

    void release(Object* obj){
        // release to the correct pool
        struct ReleaseObject : public ObjectVisitor{

            void visit(Map& obj){
                obj.~Map();
                PoolSingleton<Map>::instance().release(&obj);
            }

            void visit(MapTransient& obj){
                obj.~MapTransient();
                PoolSingleton<MapTransient>::instance().release(&obj);
            }

            void visit(Number& obj){
                obj.~Number();
                PoolSingleton<Number>::instance().release(&obj);
            }

            void visit(String& obj){
                obj.~String();
                PoolSingleton<String>::instance().release(&obj);
            }

            void visit(Array& obj){
                obj.~Array();
                PoolSingleton<Array>::instance().release(&obj);
            }

            void visit(ArrayTransient& obj){
                obj.~ArrayTransient();
                PoolSingleton<ArrayTransient>::instance().release(&obj);
            }

            void visit(Method& obj){
                obj.~Method();
                PoolSingleton<Method>::instance().release(&obj);
            }

            void visit(NativeMethod& obj){
                obj.~NativeMethod();
                PoolSingleton<NativeMethod>::instance().release(&obj);
            }

            void visit(UserData& obj){
                obj.~UserData();
                PoolSingleton<UserData>::instance().release(&obj);
            }
        };
        ReleaseObject releaser;
        obj->accept(releaser);
    }

}
