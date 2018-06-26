// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/World.hpp>

#include <compiler/Compiler.hpp>
#include <vm/VM.hpp>
#include <vm/ObjectSerializer.hpp>
#include <vm/MemoryManager.hpp>
#include <vm/ConstantsTable.hpp>

#include <misc/Exceptions.hpp>

#include <utils/files.hpp>

#include <dlfcn.h>

#ifdef BENCHMARK
#include <chrono>
#endif


namespace jupiter{


    World::World() : vm(*this){

        if ( const char* path = getenv( "JUPITERHOME" )) {
            MapStringAdapter prototypesAdapter(ConstantsTable::instance(), prototypes);
            MapStringAdapter globalsAdapter(ConstantsTable::instance(), globals);

            // init Map prototype with an empty Map
            prototypesAdapter.putAtMut("Map", make<Map>() );

            loadPrototypes(std::string(path) + "/core-types");
            //now in prototypes.at("Map") there is the real map prototype


            // Create core types globals with the right type
            globalsAdapter.putAtMut("Number", make_permanent<Number>(0) );
            globalsAdapter.putAtMut("Array", make_permanent<Array>());
            globalsAdapter.putAtMut("String", make_permanent<String>() );

            globalsAdapter.putAtMut("Map",
                                    make_permanent<Map>( static_cast<Map&>( *( getPrototype("Map") ) ) ) );

            globalsAdapter.putAtMut("Method", make_permanent<Method>());

            loadPackage(std::string(path) + "/core");

        }else{
            std::cout << "| WARNING: JUPITERHOME environment variable is not set" << std::endl;
            std::cout << "| Core library is not loaded, you probably can't do much without it" << std::endl;
            std::cout << "| Try the following command ( assuming you are in the Jupiter source code folder )" << std::endl;
            std::cout << "| export JUPITERHOME=$PWD/lib/core" << std::endl << std::endl;
        }

    }

    World::~World(){

        // close open libraries
        for(auto& pair : nativeLibs ){
            dlclose(pair.second);
        }

    }

    Object* World::getTrue(){
        static auto trueObj = getGlobal("true");
        return trueObj;
    }

    Object* World::getFalse(){
        static auto falseObj = getGlobal("false");
        return falseObj;
    }

    Object* World::getNil(){
        static auto nil = getGlobal("nil");
        return nil;
    }


    Object* World::getGlobal(const std::string& globalName){
        MapStringAdapter globalsAdapter(ConstantsTable::instance(), globals);

        return globalsAdapter.at(globalName);
    }

    Object* World::getGlobal(unsigned id){

        return globals.at(id);
    }

    Object* World::getPrototype(const std::string& prototypeName){
        MapStringAdapter prototypesAdapter(ConstantsTable::instance(), prototypes);

        return prototypesAdapter.at( prototypeName );
    }

    void World::loadPackage(const std::string& path){

        ObjectSerializer serializer(*this);
        serializer.deserialize(path, &globals);
    }

    void World::loadPrototypes(const std::string& path){

        ObjectSerializer serializer(*this);
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

    }

    Object* World::eval(Object* o){
        return vm.eval(o);
    }

}
