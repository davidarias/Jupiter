// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __POOL_H
#define __POOL_H

#include <vector>
#include <cstdlib>

#define INIT_POOL_SIZE 16384

namespace jupiter{

    template<class T>
    class Pool{
    private:
        std::vector<T*> objects;
        unsigned capacity;
    public:
        Pool() : capacity(INIT_POOL_SIZE) {
            for(unsigned i = 0; i < capacity; i++ ){
                objects.push_back(allocate());
            }

        };

        ~Pool(){
            for (auto obj : objects ){
                std::free( obj );
            }

        }

        T* allocate(){
            return reinterpret_cast<T*>( std::malloc(sizeof(T)) );
        }

        T* obtain(){
            if (objects.empty()){
                return allocate();
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
                objects.push_back(allocate());
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

    template<class T>
    class PoolSingleton : public Pool<T>{
    private:
        PoolSingleton(){}
        PoolSingleton(const PoolSingleton& ) = delete;
        void operator=(const PoolSingleton& ) = delete;
    public:
        static PoolSingleton& instance(){
            static PoolSingleton i;
            return i;
        }
    };

}

#endif
