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
        std::vector<T*> from;
        std::vector<T*> to;

        std::vector<T*> inmortal;
        ObjectPool<T> pool;

        #ifdef BENCHMARK
        unsigned gcCycles = 0;
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
            auto p = pool.obtain();

            // p->~T(); // reset/call destructor

            new(p) T(args...); // reuse old memory

            if ( from.size() == 0){
                from.push_back(p);
            }else{
                to.push_back(p);
            }


            if ( pool.empty() ){
                #ifdef BENCHMARK
                gcCycles++;
                #endif

                // dont collect this new created object!
                p->mark();

                #ifdef BENCHMARK

                auto t1 = std::chrono::high_resolution_clock::now();
                World::instance().gc();
                auto t2 = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> dt =  t2 - t1;
                double time = dt.count();
                markTime += time;
                LOG("Mark time "  << time << " gcCycle: " << gcCycles);
                #else

                World::instance().gc();

                #endif

                // execute sweep in all managers
                MemoryManagers::instance().sweep();

                if ( pool.size() < pool.getCapacity() * 0.1 ){
                    // if pool size after sweep is less than 10% of capacity,
                    // we need a bigger pool
                    pool.grow();
                    #ifdef BENCHMARK
                    LOG("GROW gcCycle " << gcCycles);
                    #endif
                }


            }
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

        void sweep(){

            if ( from.size() == 0){

                for ( auto obj : from ){

                    if (! obj->isMarked() ){

                        pool.release( obj );

                    }else{
                        obj->unmark();
                        to.push_back(obj);
                    }
                }

                // this should call destructors on every object reclaimed
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

                // this should call destructors on every object reclaimed
                to.clear();
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
