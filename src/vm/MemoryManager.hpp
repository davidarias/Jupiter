// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __MEMORYMANAGER_H
#define __MEMORYMANAGER_H

#include <misc/common.hpp>
#include <vm/World.hpp>

#ifdef BENCHMARK
#include <chrono>
#include <cxxabi.h> // get demagled names with gcc
#endif


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

            for (auto obj : objects ){
                delete obj;
            }

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
        virtual void sweep(bool full) = 0;
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
        void sweep(bool full);


    private:
        MemoryManagers();
        MemoryManagers(const MemoryManagers& ) = delete;
        void operator=(const MemoryManagers& ) = delete;
    };

    template<class T>
    class MemoryManager : public MemoryManagerInterface{
    private:
        std::vector<T*> eden;

        std::vector<T*> from;
        std::vector<T*> to;

        std::vector<T*> inmortal;
        ObjectPool<T> pool;

        unsigned gcCycles = 0;

        #ifdef BENCHMARK
        double markTime = 0;
        double sweepTime = 0;
        #endif

    public:
        static MemoryManager& instance() {
            static MemoryManager i;
            return i;
        }


        template<typename... Args>
        T* get(Args... args){

            if ( pool.empty() ){
                #ifdef BENCHMARK
                gcCycles++;

                auto t1 = std::chrono::high_resolution_clock::now();

                if (gcCycles % 15 == 0){
                    World::instance().vm.gc(true);
                }else{
                    World::instance().vm.gc(false);
                }

                auto t2 = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> dt =  t2 - t1;
                double time = dt.count();
                markTime += time;

                #else

                if (gcCycles % 15 == 0){
                    World::instance().vm.gc(true);
                }else{
                    World::instance().vm.gc(false);
                }

                #endif

                // execute sweep in all managers
                if (gcCycles % 15 == 0){
                    MemoryManagers::instance().sweep(true);
                }else{
                    MemoryManagers::instance().sweep(false);
                }

                if ( pool.size() < pool.getCapacity() * 0.1 ){
                    // if pool size after sweep is less than 10% of capacity,
                    // we need a bigger pool
                    pool.grow();

                }


            }

            auto p = pool.obtain();

            p->~T(); // reset/call destructor

            new(p) T(args...); // reuse old memory

            eden.push_back(p);
            return p;
        }

        // allocate objects that are never garbage collected
        template<typename... Args>
        T* permanent(Args... args){
            auto p = new T(args...);
            inmortal.push_back(p);
            return p;
        }


        ~MemoryManager(){

            #ifdef BENCHMARK
            int status;
            std::string tname = typeid(T).name();
            char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
            LOG("-------------------------------");
            LOG(demangled_name);
            LOG("Total GC cicles: " << gcCycles << " total Mark Time: " << markTime);

            #endif

            for (auto obj : from ){
                delete obj;
            }

            for (auto obj : to ){
                delete obj;
            }

            for (auto obj : inmortal ){
                delete obj;
            }

        }

        void sweep(bool full){

            // first sweep tenured, then we can promote from eden if necesary
            if ( full ){

                if ( to.size() == 0){

                    for ( auto obj : from ){

                        if (! obj->isMarked() ){
                            pool.release( obj );

                        }else{
                            obj->unmark();
                            to.push_back(obj);
                        }
                    }

                    from.clear();

                }else{

                    for ( auto obj : to ){

                        if (! obj->isMarked() ){
                            pool.release( obj );

                        }else{
                            obj->unmark();
                            from.push_back(obj);
                        }
                    }

                    to.clear();
                }
            }

            for ( auto obj : eden ){

                // transients can point to young objects
                // being tenured, so when adding an object
                // to a tenured transient we also mark
                // this object as tenured. when minor gc cycle
                // is executed, we move this object to tenured space
                if (! obj->isMarked() && ! obj->istenured() ){

                    pool.release( obj );

                }else{
                    obj->unmark();
                    obj->setTenured();

                    if (to.size() == 0){
                        from.push_back(obj);
                    }else{
                        to.push_back(obj);
                    }
                }
            }

            eden.clear();

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
