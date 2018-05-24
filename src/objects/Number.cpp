// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <objects/Number.hpp>

#include <vm/MemoryManager.hpp>
#include <vm/World.hpp>

namespace jupiter{

    NumberContext::NumberContext(){
        mpd_init(&mpd_context, NumberContext::DEFAULT_PRECISION );
    }

    NumberContext::NumberContext( int prec ){
        mpd_init(&mpd_context, prec );
    }

    NumberContext Number::context;

    mpd_context_t* Number::getMpdContext(){
        return &( context.mpd_context );
    }

    Number::Number()/* value( mpd_qnew() )*/ {}

    Number::Number(int64_t intvalue) {
        uint32_t status = 0;
        mpd_qset_i64( &value, intvalue, getMpdContext(), &status );
        addStatus(status);
    }

    Number::Number( std::string& stringvalue) {
        uint32_t status = 0;
        mpd_qset_string( &value, stringvalue.c_str(), getMpdContext(), &status );
        addStatus(status);
    }

    Number::~Number(){
        mpd_del( &value );
    }

    void Number::addStatus(uint32_t status){
        auto context = getMpdContext();
        context->status |= status;
        if (context->traps & status){
            char status_str[MPD_MAX_FLAG_STRING];
            mpd_snprint_flags(status_str, MPD_MAX_FLAG_STRING, status);
            std::stringstream buffer;
            buffer << "Mpdec error (" << status << "): " << std::string(status_str);
            throw buffer.str();
        }
    }

    Number* Number::operator+(Number& other){
        uint32_t status = 0;
        Number* result = MemoryManager<Number>::instance().get();
        mpd_qadd( &result->value, &value, &other.value, getMpdContext(), &status );
        addStatus(status);
        return result;
    }

    Number* Number::operator-(Number& other){
        uint32_t status = 0;
        Number* result = MemoryManager<Number>::instance().get();
        mpd_qsub( &result->value, &value, &other.value, getMpdContext(), &status );
        addStatus(status);
        return result;
    }

    Number* Number::operator*(Number& other){
        uint32_t status = 0;
        Number* result = MemoryManager<Number>::instance().get();
        mpd_qmul( &result->value, &value, &other.value, getMpdContext(), &status );
        return result;
    }

    Number* Number::operator/(Number& other){
        uint32_t status = 0;
        Number* result = MemoryManager<Number>::instance().get();
        mpd_qdiv( &result->value, &value, &other.value, getMpdContext(), &status );
        addStatus(status);
        return result;
    }

    int Number::cmp(Object& other){
        uint32_t status = 0;
        // we checked the type in the == operator

        // XXX if we use a reference here, mpdec crash with a weird error
        // using a pointer seems to be happy ¯\_(ツ)_/¯
        auto otherNumber = static_cast<Number*>(&other);
        int cmp = mpd_qcmp( &value, &otherNumber->value, &status);
        addStatus(status);
        return cmp;
    }

   int64_t Number::truncate(){
       uint32_t status = 0;
       // use static dec number
       mpd_uint_t dt[MPD_MINALLOC_MAX];
       mpd_t tmp = {MPD_STATIC|MPD_STATIC_DATA,0,0,0,MPD_MINALLOC_MAX,dt};
       mpd_qtrunc( &tmp, &value, getMpdContext(), &status );
       addStatus(status);
       int64_t intresult = mpd_qget_i64( &tmp, &status);
       addStatus(status);
       return intresult;
    }

    Object* Number::at(const std::string& selector){
        // globals are always maps
        static Map& behaviour = static_cast<Map&>( *( World::instance().globals.at("Number") ));
        return behaviour.at( selector );
    }


    std::string Number::toString(){
        char* result = mpd_to_sci(&value, 1);
        std::string buffer;
        buffer.assign(result);
        mpd_free(result);
        return buffer;
    }

    void Number::eval(Evaluator& evaluator){
        evaluator(*this);
    }

}
