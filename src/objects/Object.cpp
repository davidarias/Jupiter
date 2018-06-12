// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <objects/Object.hpp>

namespace jupiter{

    void GCObject::mark(){
        marked = true;
    }

    void GCObject::unmark(){
        marked = false;
    }

    bool GCObject::isMarked(){
        return marked;
    }

    bool GCObject::istenured(){
        return tenured;
    }

    void GCObject::setTenured(){
        tenured = true;
    }

    Object::Object(){}
    Object::~Object(){}

    bool Object::equal(Object& other){
        return this->cmp(other) == 0;
    }

    bool operator==(Object& a, Object& b){
        if ( typeid( a ) == typeid( b ) && a.equal( b ) ){
            return true;
        }
        return false;
    }

    bool operator!=(Object& a, Object& b){
        return !( a == b );
    }

    bool operator>(Object& a, Object& b){
        if ( typeid( a ) != typeid( b ) ) throw "Diferent types are not comparable";
        if (  a.cmp( b ) > 0 ){
            return true;
        }
        return false;
    }
    bool operator<(Object& a, Object& b){
        if ( typeid( a ) != typeid( b ) ) throw "Diferent types are not comparable";
        if ( a.cmp( b ) < 0 ){
            return true;
        }
        return false;
    }

    bool operator<=(Object& a, Object& b){
        if ( typeid( a ) != typeid( b ) ) throw "Diferent types are not comparable";
        if ( a.cmp(b) < 0 || a.equal(b) ) return true;
        return false;
    }

    bool operator>=(Object& a, Object& b){
        if ( typeid( a ) != typeid( b ) ) throw "Diferent types are not comparable";
        if ( a.cmp(b) > 0 || a.equal(b) ) return true;
        return false;
    }



}
