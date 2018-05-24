// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/MemoryManager.hpp>


namespace jupiter{

    MemoryManagers::MemoryManagers(){}

    void MemoryManagers::add(MemoryManagerInterface* manager){
        managers.push_back(manager);
    }

    void MemoryManagers::sweep(){
        for( auto manager : managers){
            manager->sweep();
        }
    }

}
