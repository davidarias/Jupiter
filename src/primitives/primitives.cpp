// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <primitives/primitives.hpp>

#include <objects/Objects.hpp>
#include <vm/World.hpp>

#include <primitives/functions.hpp>

namespace jupiter{

    Primitives::Primitives(){
        // common
        add("print", 1, print);
        add("endl", 0, endl );

        add("isIdenticalTo", 1, isIdenticalTo);
        add("equals",          1, equals ) ;
        add("greater",         1, greater ) ;
        add("greaterOrEqual",  1, greaterOrEqual ) ;
        add("less",            1, less ) ;
        add("lessOrEqual",     1, lessOrEqual ) ;

        // numbers
        add("plus",      1, plus ) ;
        add("minus",     1, minus ) ;
        add("multiply",  1, multiply ) ;
        add("divide",    1, divide ) ;
        add("sqrt",      0, sqrt ) ;

        add("random",     0, random ) ;

        // strings
        add("stringConcat", 1, stringConcat ) ;

        // arrays
        add("arrayAt",            1, arrayAt ) ;
        add("arrayPush",          1, arrayPush ) ;
        add("arrayTake",          1, arrayTake ) ;
        add("arrayDrop",          1, arrayDrop ) ;
        add("arraySize",          0, arraySize ) ;
        add("arrayFormatString",  1, arrayFormatString ) ;

        add("arrayTransient",        0, arrayTransient ) ;
        add("arrayTransientPersist", 0, arrayTransientPersist ) ;
        add("arrayTransientPush",    1, arrayTransientPush ) ;

        // maps
        add("mapAt",               1, mapAt ) ;
        add("mapAtPut",            2, mapAtPut ) ;
        add("mapTransient",        0, mapTransient );
        add("mapTransientPersist", 0, mapTransientPersist ) ;
        add("mapTransientAtPut",   2, mapTransientAtPut ) ;

        // methods
        add("eval0",  0, methodEval );
        add("eval1",  1, methodEval );
        add("eval2",  2, methodEval );
        add("eval3",  3, methodEval );
        add("printBytecode", 0, methodPrintByteCode );


        add("loadPath", 1, loadPath );
        add("loadNative", 1, loadNative );
    }

    void Primitives::add(std::string name, unsigned arity, NativeFunction primitiveFunction){
        NativeMethod primitive(primitiveFunction, arity);
        unsigned index = primitives.size();
        primitives.push_back(primitive);
        primitivesMap[name] = index;
    }

    Object* Primitives::get(std::string& name){
        try{
            return &( primitives.at( primitivesMap.at(name) ) );
        }catch(std::exception& e){
            throw  "Primitive \'" + name + "\' not found.";
        }

    }


}
