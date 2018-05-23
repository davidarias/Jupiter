// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

#include <exception>
#include <stdexcept>

namespace jupiter{

    class DisturbanceInTheForce : public std::runtime_error {
    public:
        DisturbanceInTheForce( std::string what ) : runtime_error( what ){}
    };

    class SyntaxError : public DisturbanceInTheForce {
    public:
        SyntaxError( std::string what ) : DisturbanceInTheForce( "(Syntax Error) " + what ){}
    };


    class CompilerError : public DisturbanceInTheForce {
    public:
        CompilerError( std::string what ) : DisturbanceInTheForce( what ){}
    };

    class RuntimeException : public DisturbanceInTheForce {
    public:
        RuntimeException ( std::string what ) : DisturbanceInTheForce( "(Runtime error) " + what ){}
    };

    class KeyNotFound : public RuntimeException{
    public:
        KeyNotFound ( std::string key ) : RuntimeException( "Key '" + key + "\' not found" ){}
    };

    class SelectorNotFound : public RuntimeException {
    public:
        SelectorNotFound ( std::string key ) : RuntimeException( "Selector \'" + key + "\' not found in object" ){}
    };

}

#endif
