#ifndef __STRING_H
#define __STRING_H

#include <misc/common.hpp>

#include <objects/Object.hpp>

namespace jupiter{

    class String : public Object{
    private:
        std::string value;
    protected:
        int cmp(Object& other);
    public:
        String();
        String(const std::string& value);

        void accept(ObjectVisitor&);

        std::string& getValue();

        String* operator+(String& other);

        std::string toString();

    };
}
#endif
