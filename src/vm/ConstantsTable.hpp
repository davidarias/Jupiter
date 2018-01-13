// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __CONSTANTS_TABLE_H
#define __CONSTANTS_TABLE_H

#include <misc/common.hpp>

namespace jupiter{

    class Object;

    class ConstantsTable{
        std::unordered_map<std::string, unsigned> numbers;
        std::unordered_map<std::string, unsigned> strings;

        std::vector<Object*> constants;

    public:
        static ConstantsTable& instance(){
            static ConstantsTable i;
            return i;
        }

        unsigned number(const std::string& number);
        unsigned string(const std::string& string);
        Object* get(unsigned index);

    private:
        ConstantsTable();
        ConstantsTable(const ConstantsTable&) = delete;
        void operator=(const ConstantsTable&) = delete;
    };

}


#endif
