#include <objects/String.hpp>

#include <vm/MemoryManager.hpp>
#include <vm/World.hpp>

namespace jupiter{

    String::String() {}
    String::String(const std::string& value): value(value) {}

    void String::accept(ObjectVisitor& visitor){
        visitor.visit(*this);
    }

    std::string& String::getValue(){
        return value;
    }

    int String::cmp(Object& other){
        // we checked the type in the == operator
        auto otherString = static_cast<String&>(other);
        return value.compare(otherString.value);;
    }

    String* String::operator+(String& other){
        return MemoryManager<String>::instance().get(value + other.value);
    }

    Object* String::at(const std::string& selector){
        // globas are always maps
        static Map& behaviour =  static_cast<Map&>( *( World::instance().prototypes.at("String") ));
        return behaviour.at( selector );
    }

    std::string String::toString(){
        return value;
    }


}
