// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __SYMBOLTABLE_H
#define __SYMBOLTABLE_H

#include <misc/common.hpp>

namespace jupiter{

    class SymbolTable{

    private:
        std::unordered_map<std::string, unsigned> symbolTable;
        unsigned nextIndex;

    public:
        SymbolTable();

        unsigned getOrCreate(std::string& symbol);
        unsigned size();
        int find(std::string& symbol);

        void reset();

    };
}
#endif
