// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/World.hpp>

#include <compiler/Compiler.hpp>
#include <vm/VM.hpp>
#include <vm/ObjectSerializer.hpp>

#include <vm/MemoryManager.hpp>

#ifdef BENCHMARK
#include <chrono>
#endif


namespace jupiter{


    World::World(){

        if ( const char* path = getenv( "JUPITERHOME" )) {
            loadPackage(std::string(path) + "/src");
        }else{
            std::cout << "| WARNING: JUPITERHOME environment variable is not set" << std::endl;
            std::cout << "| Core library is not loaded, you probably can't do much without it" << std::endl;
            std::cout << "| Try the following command ( assuming you are in the Jupiter source code folder )" << std::endl;
            std::cout << "| export JUPITERHOME=$PWD/lib/core" << std::endl << std::endl;
        }

    }

    Object* World::getTrue(){
        static auto trueObj = globals.at("true");
        return trueObj;
    }

    Object* World::getFalse(){
        static auto falseObj = globals.at("false");
        return falseObj;
    }

    Object* World::getNil(){
        static auto nil = globals.at("nil");
        return nil;
    }

    void World::loadPackage(const std::string& path){
        std::cout << "Loading package" << path << std::endl << std::endl;

        ObjectSerializer serializer;
        serializer.deserialize(path, &globals);
    }


    void World::eval(std::string source){
        // TODO refactor exception capture ( also in Method::Method )

        auto method = Compiler::compile( source );

        if ( method ){
            #ifdef BENCHMARK

            auto t1 = std::chrono::high_resolution_clock::now();

            auto result = vm.eval(method);
            auto t2 = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> executionTime =  t2 - t1;

            LOG("BENCHMARK: " << executionTime.count() << " s");

            #else

            auto result = vm.eval(method);

            #endif

            if ( result != nullptr ){
                std::cout << result->toString() << std::endl;
            }
            // vm.pop();

        }
        // TODO pop last value of stack
        vm.gc(true);

    }

    Object* World::eval(Object* o){
        return vm.eval(o);
    }

    Object* World::eval(Object* o, Object* self){
        return vm.eval(o, self);
    }

}
