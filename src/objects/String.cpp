#include "String.hpp"

#include <vm/World.hpp>

namespace jupiter{

    String::String(const std::string& value): value(value) {}

    std::string& String::getValue(){
        return value;
    }

    int String::cmp(Object& other){
        // we checked the type in the == operator
        auto otherString = static_cast<String&>(other);
        return value.compare(otherString.value);;
    }

    Object* String::at(const std::string& selector){
        // globas are always maps
        static Map& behaviour =  static_cast<Map&>( *( World::instance().globals.at("String") ));
        return behaviour.at( selector );
    }

    std::string String::toString(){
        return value;
    }

    void String::eval(Evaluator& evaluator){
        evaluator(*this);
    }


}
