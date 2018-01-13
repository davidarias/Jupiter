// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __MEMORYMANAGER_H
#define __MEMORYMANAGER_H

#include <misc/common.hpp>
#include <vm/World.hpp>

#include <list>

namespace jupiter{

    class Object;
    class Number;
    class String;
    class Array;
    class Method;
    class CompiledMethod;
    class UpValue;


    class MemoryManager{
    private:
        std::list<Object*> allocatedOjects;
        std::list<Object*> constants;

        unsigned counter = 0;
    public:
        static MemoryManager& instance() {
            static MemoryManager i;
            return i;
        }

        ~MemoryManager();

        void sweep();

        template<class T, typename... Args>
        T* get(Args... args){
            auto p = new T(args...);
            allocatedOjects.push_back(p);
            counter++;
            // TODO improve the trigger of GC cycle usign a configurable size
            if ( counter > 1038336){
                // dont collect this new created object!
                p->mark();
                World::instance().gc();
                counter = 0;
            }
            return p;
        }

        // allocate objects that are never garbage collected
        template<class T, typename... Args>
        T* permanent(Args... args){
            auto p = new T(args...);
            constants.push_back(p);
            return p;
        }

        void reclaim(Object* obj);

    private:
        MemoryManager();
        MemoryManager(const MemoryManager& ) = delete;
        void operator=(const MemoryManager& ) = delete;

    };
}
#endif
