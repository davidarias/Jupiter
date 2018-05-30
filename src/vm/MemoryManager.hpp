// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __MEMORYMANAGER_H
#define __MEMORYMANAGER_H

#include <misc/common.hpp>
#include <vm/World.hpp>

#include <list>

#include <chrono>

// #include <cxxabi.h>

#define INIT_POOL_SIZE 16384

namespace jupiter{

    class Object;
    class Number;
    class String;
    class Array;
    class Method;
    class CompiledMethod;
    class UpValue;

    template<class T>
    class ObjectPool{

    private:
        std::vector<T*> objects;
        unsigned capacity;
    public:
        ObjectPool() : capacity(INIT_POOL_SIZE) {
            for(unsigned i = 0; i < capacity; i++ ){
                objects.push_back(new T);
            }

        };

        ~ObjectPool(){

            auto it = objects.begin();
            while (it != objects.end() ){
                if (*it != nullptr){
                    delete (*it);
                }
                it++;
            }

            objects.clear();
        }

        T* obtain(){
            if (objects.empty()){
                return new T;
            } else{
                T* object = objects.back();
                objects.pop_back();
                return object;
            }
        }

        void grow(){
            capacity *= 2;
            objects.reserve(capacity);
            // LOG("Capacity: " << capacity);
            for(unsigned i = objects.size() - 1; i < capacity; i++){
                objects.push_back(new T);
            }
        }

        bool empty(){
            return objects.empty();
        }

        unsigned getCapacity(){
            return capacity;
        }

        unsigned size(){
            return objects.size();
        }

        void release(T* object){
            objects.push_back(object);
        }


    };

    class MemoryManagerInterface{
    public:
        virtual void sweep() = 0;
    };

    // contain all the memory manager instances
    // to execute a sweep of all objects types
    // since the mark phase cannot separate types
    class MemoryManagers{

    private:
        std::vector<MemoryManagerInterface*> managers;

    public:
        static MemoryManagers& instance() {
            static MemoryManagers i;
            return i;
        }

        void add(MemoryManagerInterface* manager);
        void sweep();


    private:
        MemoryManagers();
        MemoryManagers(const MemoryManagers& ) = delete;
        void operator=(const MemoryManagers& ) = delete;
    };

    template<class T>
    class MemoryManager : public MemoryManagerInterface{
    private:
        std::list<T*> allocatedOjects;
        std::list<T*> constants;
        ObjectPool<T> pool;

    public:
        static MemoryManager& instance() {
            static MemoryManager i;
            return i;
        }


        template<typename... Args>
        T* get(Args... args){
            auto p = pool.obtain();

            p->~T(); // reset/call destructor

            new(p) T(args...); // reuse old memory

            allocatedOjects.push_back(p);

            if ( pool.empty() ){

                // dont collect this new created object!
                p->mark();
                World::instance().gc();

                // execute sweep in all managers
                MemoryManagers::instance().sweep();

                if ( pool.size() < pool.getCapacity() * 0.1 ){
                    // if pool size after sweep is less than 10% of capacity,
                    // we need a bigger pool
                    pool.grow();
                }


            }
            return p;
        }

        // allocate objects that are never garbage collected
        template<typename... Args>
        T* permanent(Args... args){
            auto p = new T(args...);
            constants.push_back(p);
            return p;
        }


        ~MemoryManager(){
            // int status;
            // std::string tname = typeid(T).name();
            // char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);

            // LOG("Total GC cicles: " << gcCycles << " for " << demangled_name);

            auto it1 = allocatedOjects.begin();
            while (it1 != allocatedOjects.end() ){
                if (*it1 != nullptr){
                    delete (*it1);
                }
                it1++;
            }

            allocatedOjects.clear();

            auto it2 = constants.begin();
            while (it2 != constants.end() ){
                if (*it2 != nullptr){
                    delete (*it2);
                }
                it2++;
            }

            constants.clear();
        }

        void sweep(){

            auto it = allocatedOjects.begin();
            while (it != allocatedOjects.end() ){
                auto obj = (*it);

                if (! obj->isMarked() ){

                    pool.release( obj );
                    allocatedOjects.erase(it++);

                }else{
                    obj->unmark();
                    ++it;
                }

            }
        }

    private:
        MemoryManager(){
            // save this instance
            MemoryManagers::instance().add(this);
        };
        MemoryManager(const MemoryManager& ) = delete;
        void operator=(const MemoryManager& ) = delete;

    };


}
#endif
