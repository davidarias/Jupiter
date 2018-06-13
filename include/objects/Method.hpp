// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __METHOD_H
#define __METHOD_H


#include <objects/Object.hpp>


namespace jupiter{

    class CompiledMethod;

    class Method : public Object {
        friend class Frame;
    private:
        std::string name;
        std::string signature;
        std::string source;
        std::shared_ptr<CompiledMethod> compiledMethod;
        Object* self;

        // TODO optimize this ( maybe using a sparse vector? )
        std::unordered_map<unsigned, Object*> upvalues;

    protected:
        int cmp(Object& other);
        bool equal(Object& other);
    public:
        Method();
        Method(std::string& name, std::string& signature, std::string& source,
               std::shared_ptr<CompiledMethod> compiledMethod);

        Method(std::shared_ptr<CompiledMethod> compiledMethod);
        ~Method();

        void mark();

        std::string& getName();
        std::shared_ptr<CompiledMethod> getCompiledMethod();

        Object* at(const std::string& selector);
        std::string toString();
        void eval(Evaluator& evaluator);


    };

}

#endif
