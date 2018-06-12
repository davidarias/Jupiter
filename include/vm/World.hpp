// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __WORLD_H
#define __WORLD_H

#include <misc/common.hpp>
#include <vm/VM.hpp>
#include <objects/Objects.hpp>

namespace jupiter{

    class World{
        // the primitive eval function can acces directly the vm
        // friend Object* methodEval(PrimitiveArguments& arguments);
    private:
        World();
        ~World();
        World(const World& ) = delete;
        void operator=(const World& ) = delete;

        std::unordered_map<std::string, void*> nativeLibs;

    public:
        Map globals;
        Map prototypes;
        VM vm;

        Object* getTrue();
        Object* getFalse();
        Object* getNil();

        static World& instance() {
            static World i;
            return i;
        }

        Object* getNativeExtensionMethod(const std::string& lib, const std::string& name);

        void loadPrototypes(const std::string& path);
        void loadPackage(const std::string& path);
        void loadNative(const std::string& path);

        void eval(std::string);
        Object* eval(Object* o);
        Object* eval(Object* o, Object* self);

    };
}
#endif
