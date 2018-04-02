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
        String(const std::string& value);

        std::string& getValue();

        String* operator+(String& other);

        Object* at(const std::string& selector);
        std::string toString();
        void eval(Evaluator& evaluator);

    };
}
#endif
