// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/MemoryManager.hpp>

#include <objects/Objects.hpp>
#include <objects/CompiledMethod.hpp>

namespace jupiter{

    MemoryManager::MemoryManager(){}

    MemoryManager::~MemoryManager(){
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

    void MemoryManager::sweep(){
        unsigned released = 0;
        auto it = allocatedOjects.begin();
        while (it != allocatedOjects.end() ){
            if (! (*it)->isMarked() ){
                delete (*it);
                allocatedOjects.erase(it++);
                released++;
            }else{
                (*it)->unmark();
                ++it;
            }

        }

        // LOG("GC cycle. collected: " << released);
    }

    void MemoryManager::reclaim(Object* obj){
        delete obj;
    }
}
