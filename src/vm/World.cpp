// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/World.hpp>

#include <compiler/Compiler.hpp>
#include <vm/VM.hpp>
#include <vm/ObjectSerializer.hpp>

#include <vm/MemoryManager.hpp>

#include <misc/Exceptions.hpp>

#include <utils/files.hpp>

#include <dlfcn.h>

#ifdef BENCHMARK
#include <chrono>
#endif


namespace jupiter{


    World::World() : vm(globals){

    }

    World::~World(){

        // close open libraries
        for(auto& pair : nativeLibs ){
            dlclose(pair.second);
        }

    }


    void World::init(){

        if ( ! initialized ){
            if ( const char* path = getenv( "JUPITERHOME" )) {

                // init Map prototype with an empty Map
                prototypes.putAtMut("Map", MemoryManager<Map>::instance().get() );

                loadPrototypes(std::string(path) + "/core-types");
                //now in prototypes.at("Map") there is the real map prototype


                // Create core types globals with the right type
                globals.putAtMut("Number", MemoryManager<Number>::instance().permanent(0) );
                globals.putAtMut("Array", MemoryManager<Array>::instance().permanent() );
                globals.putAtMut("String", MemoryManager<String>::instance().permanent() );

                globals.putAtMut("Map",
                                 MemoryManager<Map>::instance()
                                 .permanent( static_cast<Map&>( *( prototypes.at("Map") ) ) ) );

                globals.putAtMut("Method", MemoryManager<Method>::instance().permanent());

                loadPackage(std::string(path) + "/core");

            }else{
                std::cout << "| WARNING: JUPITERHOME environment variable is not set" << std::endl;
                std::cout << "| Core library is not loaded, you probably can't do much without it" << std::endl;
                std::cout << "| Try the following command ( assuming you are in the Jupiter source code folder )" << std::endl;
                std::cout << "| export JUPITERHOME=$PWD/lib/core" << std::endl << std::endl;
            }

            initialized = true;
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

        ObjectSerializer serializer;
        serializer.deserialize(path, &globals);
    }

    void World::loadPrototypes(const std::string& path){

        ObjectSerializer serializer;
        serializer.deserialize(path, &prototypes);
    }

    void World::loadNative(const std::string& path){

        void* handle = dlopen (path.c_str(), RTLD_LAZY);

        if (!handle) {

            std::ostringstream message;
            message << "Error loading native library: " << path;
            message << " | dlopen error: " << dlerror();

            throw RuntimeException(message.str());

        }else{

            auto fileName = getFileName(path);
            LOG("native lib filename " << fileName );
            nativeLibs[fileName] = handle;

        }

    }

    Object* World::getNativeExtensionMethod(const std::string& lib, const std::string& name){
        auto librayHandle = nativeLibs.at(lib);

        // FIXME clear memory
        auto sym = dlsym( librayHandle, name.c_str() );
        auto method =
            new NativeMethod (reinterpret_cast<NativeFunction>( sym ), 0 );

        char *error = NULL;
        if ( (error = dlerror() ) != NULL)  {
            std::ostringstream message;
            message << "Error loading native method in library: " << lib;
            message << " | dlerror: " << error;

            throw RuntimeException(message.str());
        }

        return method;
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

}
