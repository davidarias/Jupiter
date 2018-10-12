// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __WORLD_H
#define __WORLD_H

#include <misc/common.hpp>
#include <vm/VM.hpp>
#include <vm/ConstantsTable.hpp>
#include <objects/Objects.hpp>

#include <primitives/primitives.hpp>
#include <extensions/NativeLibraries.hpp>

namespace jupiter{


    class World{
        friend class GC;
    private:

        Primitives primitives;
        NativeLibraries nativeLibs;

        Map globals;
        Map prototypes;
        VM vm;

    public:
        ConstantsTable constantsTable;

        World();
        ~World();

        Object* getTrue();
        Object* getFalse();
        Object* getNil();

        Object* getGlobal(const std::string& global);
        Object* getGlobal(unsigned id);
        Object* getPrototype(const std::string& prototypeName);

        void loadCore(const std::string& path);

        void loadPrototypes(const std::string& path);
        void loadPackage(const std::string& path);
        void loadNative(const std::string& path);

        void eval(std::string);
        Object* eval(Object* o);
        Object* eval(Method& method);

        Object* compile(std::string& source);
        std::tuple<std::string, Object*> compile(std::string& signature, std::string& source);

    };
}
#endif
