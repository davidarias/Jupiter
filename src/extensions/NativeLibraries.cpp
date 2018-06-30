// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <extensions/NativeLibraries.hpp>
#include <misc/Exceptions.hpp>
#include <utils/files.hpp>
#include <objects/NativeMethod.hpp>
#include <memory/memory.hpp>

#include <dlfcn.h>
#include <sstream>

namespace jupiter{

    NativeLibraries::NativeLibraries(){}

    NativeLibraries::~NativeLibraries(){
        for(auto& pair : nativeLibs ){
            dlclose(pair.second);
        }
    }

    void NativeLibraries::load(const std::string& path){
        void* handle = dlopen (path.c_str(), RTLD_LAZY);

        if (!handle) {

            std::ostringstream message;
            message << "Error loading native library: " << path;
            message << " | dlopen error: " << dlerror();

            throw RuntimeException(message.str());

        }else{

            auto fileName = getFileName(path);
            nativeLibs[fileName] = handle;

        }
    }

    Object* NativeLibraries::getMethod(const std::string& lib, const std::string& name){
        auto librayHandle = nativeLibs.at(lib);

        auto sym = dlsym( librayHandle, name.c_str() );
        auto method =
            make_permanent<NativeMethod>(reinterpret_cast<NativeFunction>( sym ), 0 );

        char *error = NULL;
        if ( (error = dlerror() ) != NULL)  {
            std::ostringstream message;
            message << "Error loading native method in library: " << lib;
            message << " | dlerror: " << error;

            throw RuntimeException(message.str());
        }

        return method;
    }

}
