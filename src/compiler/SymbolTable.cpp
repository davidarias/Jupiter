// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <compiler/SymbolTable.hpp>

namespace jupiter{

    SymbolTable::SymbolTable() : nextIndex( 0 ) {}

    unsigned SymbolTable::getOrCreate(std::string& symbol){
        auto mapIterator = symbolTable.find( symbol );

        if ( mapIterator == symbolTable.end() ){
            auto index = nextIndex;
            symbolTable[symbol] = index; nextIndex++;
            return index;

        }else{

            return mapIterator->second;
        }

    }

    unsigned SymbolTable::size(){
        return nextIndex;
    }

    int SymbolTable::find(std::string& symbol){
        auto mapIterator = symbolTable.find( symbol );
        if ( mapIterator == symbolTable.end() ){
            return -1;
        }else{
            return mapIterator->second;
        }
    }

    void SymbolTable::reset(){
        symbolTable.clear();
        nextIndex = 0;
    }
}
