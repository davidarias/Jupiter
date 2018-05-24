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
        char* initMem;
        std::vector<T*> objects;

    public:
        ObjectPool(){
            // initMem = new char[sizeof(T) * 4096];

            for(unsigned i = 0; i < 16384; i++ ){
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
            unsigned capacity = objects.capacity() * 2;
            objects.reserve(capacity);
            // LOG("Capacity: " << capacity);
            for(unsigned i = 0; i < capacity; i++){
                objects.push_back(new T);
            }
        }

        bool empty(){
            return objects.empty();
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

        unsigned gcCycles = 0;

        std::chrono::time_point<std::chrono::high_resolution_clock> lastCycle;
        double cycleDtAverage = 0;
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
                gcCycles++;


                auto time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> dt = time - lastCycle;
                lastCycle = std::chrono::high_resolution_clock::now();

                cycleDtAverage = (dt.count() + cycleDtAverage) / gcCycles;

                // adjust pool size usign average gc time
                // if ( cycleDtAverage < 0.1 ){
                //     LOG("dt average " << cycleDtAverage);
                //     pool.grow();
                //     LOG("pool size " << pool.size());

                //     cycleDtAverage = 0;
                //     gcCycles = 0;
                // }


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
            lastCycle = std::chrono::high_resolution_clock::now();
            // save this instance
            MemoryManagers::instance().add(this);
        };
        MemoryManager(const MemoryManager& ) = delete;
        void operator=(const MemoryManager& ) = delete;

    };


}
#endif
