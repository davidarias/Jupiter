// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __NATIVE_LIBRARIES_H
#define __NATIVE_LIBRARIES_H

#include <unordered_map>

namespace jupiter{

    class Object;

    class NativeLibraries{
    private:
        std::unordered_map<std::string, void*> nativeLibs;
    public:
        NativeLibraries();
        ~NativeLibraries();
        void load(const std::string& path);
        Object* getMethod(const std::string& lib, const std::string& name);
    };

}

#endif
