// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __MEMORY_H
#define __MEMORY_H

#include <misc/common.hpp>
#include <vm/World.hpp>

#include <memory/Pool.hpp>
#include <memory/GC.hpp>

#ifdef BENCHMARK
#include <chrono>
#include <cxxabi.h> // get demagled names with gcc
#endif


namespace jupiter{

    template<class T>
    T* allocate(){
        static auto& pool = PoolSingleton<T>::instance();
        static auto& gc = GC::instance();

        if ( pool.empty() ){
            gc.collect();

            if ( pool.size() < pool.getCapacity() * 0.1 ){
                // if pool size after collect is less than 10% of capacity,
                // we need a bigger pool
                pool.grow();
            }
        }

        auto p = pool.obtain();
        gc.add(p);

        return p;
    }

    void release(Object* obj);

    template<class T, typename... Args>
    T* make(Args... args){
        auto p = allocate<T>();
        return new(p) T(args...);

    }

    template<class T>
    class MemoryManagerPermanent{
        // allocate objects that are never garbage collected
    private:
        std::vector<T*> inmortal;
    public:
        MemoryManagerPermanent(){}

        T* get(){
            auto p = reinterpret_cast<T*>( std::malloc(sizeof(T)) );
            inmortal.push_back(p);
            return p;
        }


        ~MemoryManagerPermanent(){
            for (auto obj : inmortal ){
                std::free(obj);
            }
        }
    };

    template<class T, typename... Args>
    T* make_permanent(Args... args){
        static MemoryManagerPermanent<T> permanent;
        auto p = permanent.get();
        return new(p) T(args...);
    }

}
#endif
