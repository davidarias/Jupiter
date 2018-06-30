// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __GC_H
#define __GC_H

#include <vector>

namespace jupiter{

    class Object;
    class World;

    class GC{
    private:
        std::vector<Object*> eden;

        std::vector<Object*> from;
        std::vector<Object*> to;

        unsigned cycles = 0;
        #ifdef BENCHMARK
        double sweepTime = 0;
        double markTime = 0;
        #endif

        World* world; // to trigger mark phase

        void mark(bool full);
        void sweep(bool full);

        GC();
        ~GC();
        GC(const GC& ) = delete;
        void operator=(const GC& ) = delete;

    public:

        static GC& instance() {
            static GC i;
            return i;
        }

        void setWorld(World* world);

        void add(Object* obj);
        void collect();
    };

}

#endif
