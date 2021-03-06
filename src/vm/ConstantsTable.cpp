// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/ConstantsTable.hpp>

#include <misc/common.hpp>
#include <objects/Objects.hpp>
#include <memory/memory.hpp>

namespace jupiter{

    ConstantsTable::ConstantsTable(){}

    unsigned ConstantsTable::number(const std::string& number){
        auto it = numbers.find(number);
        if ( it == numbers.end() ){
            auto index = constants.size();
            auto obj = make_permanent<Number>( number );
            constants.push_back(obj);
            numbers[number] = index;
            return index;
        }else{
            return it->second;
        }
    }

    unsigned ConstantsTable::string(const std::string& string){
        auto it = strings.find(string);
        if ( it == strings.end() ){
            auto index = constants.size();
            auto obj = make_permanent<String>( string );
            constants.push_back(obj);
            strings[string] = index;
            return index;
        }else{
            return it->second;
        }
    }

    Object* ConstantsTable::get(unsigned index){
        return constants[index];
    }

}
