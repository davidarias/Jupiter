// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __NUMBER_H
#define __NUMBER_H

#include <misc/common.hpp>
#include <objects/Object.hpp>

#include <mpdecimal.h>

namespace jupiter{

    class NumberContext{
        friend class Number;
    private:
        mpd_context_t mpd_context;
    public:

        static const int DEFAULT_PRECISION = 16;

        NumberContext();
        NumberContext( int prec );
    };

    class Number : public Object {
    private:
        static const int ALLOC = 2;

        mpd_uint_t dt[ALLOC];
        mpd_t value = {MPD_STATIC|MPD_STATIC_DATA,0,0,0,ALLOC,dt};

        void addStatus(uint32_t status);
    protected:
        int cmp(Object& other);
    public:

        static NumberContext context;
        static mpd_context_t* getMpdContext();

        static Number* random();

        Number();
        Number( int64_t value );
        Number( std::string& value);

        ~Number();

        Number* operator+(Number& other);
        Number* operator-(Number& other);
        Number* operator*(Number& other);
        Number* operator/(Number& other);

        Number* sqrt();

        int64_t truncate();

        Object* at(const std::string& selector);
        std::string toString();
        void eval(Evaluator& evaler);

    };

}
#endif
