// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
#ifndef __FORMAT_H
#define __FORMAT_H

#include <string>
#include <sstream>

#define INT_INDEX_BUFFER_SIZE 10

template<class Args>
std::string format(const std::string& formatString, Args& args){
    std::ostringstream out;
    auto it = formatString.begin();
    auto end = formatString.end();

    unsigned openBrackets = 0;

    while( it != end ){
        char c = *it;
        if ( c == '{'){
            char argNameBuffer[INT_INDEX_BUFFER_SIZE];
            unsigned i = 0;
            ++openBrackets;
            ++it;

            while( it != end ){
                if ( *it == '}'){
                    --openBrackets;
                    break;
                }
                if ( i >= INT_INDEX_BUFFER_SIZE ){
                    throw std::runtime_error("Format index out of range");
                }
                char c = *it;
                argNameBuffer[i] = c;
                ++i;
                ++it;
            }
            // TODO think how detect when key is integer and when key is string
            auto index = std::stoi( argNameBuffer );
            // TODO think how to do this generic (avoid toString)
            out << args.at( index )->toString();
        }else{
            out << c;
        }
        ++it;

    }

    return out.str();
}

#endif
