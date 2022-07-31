#include "../json_serialization.hpp"
#include "../json_writer.hpp"
#include <iostream>

struct Mom
{
    int age;
    std::string name;
    pson::Value items;
    pson::Object object;

    Mom(): age(40), name("Taylor"), items(pson::JSON_OBJECT)
    {}

    Serialize_Start
        Serialize_Insert(age)
        Serialize_Insert(name)
        Serialize_Insert(items)
        Serialize_Insert(object)
    Serialize_End
};

struct Tank
{
    Mom m;
    pson::Array array;

    Serialize_Start
        Serialize_Insert(m)
        Serialize_Insert(array)
    Serialize_End
};

int main()
{
    std::cout << Tank().Serialize().print() << std::endl;
}