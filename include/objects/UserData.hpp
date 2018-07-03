// Copyright (C) 2018 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef __USERDATA_H
#define __USERDATA_H

#include <objects/Object.hpp>

namespace jupiter{


    class UserData : public Object{
    private:
        void* data;

    public:
        UserData(void* data);

        void* getData();
    };

}

#endif
