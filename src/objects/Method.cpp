// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <objects/Method.hpp>
#include <objects/Map.hpp>
#include <objects/CompiledMethod.hpp>

#include <vm/World.hpp>

namespace jupiter{

    Method::Method() {}

    Method::Method(std::string& name, std::string& signature, std::string& source,
                   std::shared_ptr<CompiledMethod> compiledMethod)
        : name(name), signature(signature), source(source), compiledMethod(compiledMethod), self(nullptr) {}

    Method::Method(std::shared_ptr<CompiledMethod> compiledMethod)
        : compiledMethod(compiledMethod), self(nullptr){}

    Method::~Method(){}

    void Method::accept(ObjectVisitor& visitor){
        visitor.visit(*this);
    }

    void Method::mark(){
        marked = true;

        if ( self != nullptr ) self->mark();

        for(auto& pair : upvalues){
            auto upvalue = pair.second;
            upvalue->mark();

        }
    }

    std::shared_ptr<CompiledMethod> Method::getCompiledMethod(){
        return compiledMethod;
    }


    bool Method::equal(Object& other){
        if (&other == this) return true;
        return false;
    }

    int Method::cmp(Object&){
        throw "Methods cannot be compared";
    }

    std::string& Method::getName(){
        return name;
    }

    std::string Method::toString(){
        std::ostringstream buffer;
        buffer << "Method " << this;
        return buffer.str();
    }

}
