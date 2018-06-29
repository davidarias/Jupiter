// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <memory/GC.hpp>
#include <memory/memory.hpp>

#include <vm/World.hpp>

namespace jupiter{

    GC::GC(){}

    GC::~GC(){
        for (auto obj : eden ){
            std::free(  obj );
        }

        for (auto obj : from ){
            std::free( obj );
        }

        for (auto obj : to ){
            std::free( obj );
        }

    }

    void GC::setWorld(World* world){
        world = world;
    }

    void GC::mark(bool full){
        world->vm.mark(full);
    }

    void GC::sweep(bool full){

        if ( full ){

            if ( to.size() == 0){

                for ( auto obj : from ){

                    if (! obj->isMarked() ){
                        release( obj );

                    }else{
                        obj->unmark();
                        to.push_back(obj);
                    }
                }

                from.clear();

            }else{

                for ( auto obj : to ){

                    if (! obj->isMarked() ){
                        release( obj );

                    }else{
                        obj->unmark();
                        from.push_back(obj);
                    }
                }

                to.clear();
            }
        }

        for ( auto obj : eden ){

            if (! obj->isMarked() ){

                release( obj );

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

    void GC::add(Object* obj){
        eden.push_back(obj);
    }

    void GC::collect(){
        if ( cycles % 15){
            mark(true);
            sweep(true);
        }else{
            mark(false);
            sweep(false);
        }
    }
}
