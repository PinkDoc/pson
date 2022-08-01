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


    DeSerialize_Start
        DeSerialize_Fetch(age)
        DeSerialize_Fetch(name)
        DeSerialize_Fetch(items)
        DeSerialize_Fetch(object)
    DeSerialize_End
};

struct Tank
{
    Mom m;
    pson::Array array;

    Serialize_Start
        Serialize_Insert(m)
        Serialize_Insert(array)
    Serialize_End

    DeSerialize_Start
        DeSerialize_Fetch(m)
        DeSerialize_Fetch(array)
    DeSerialize_End
};

int main()
{
    Tank t;
    t.m.age = 1989;
    t.m.name = "Oly";
    auto json_data = t.Serialize();
    Tank t2;
    t2.DeSerlizeFromValue(json_data);
    assert(t2.m.name == "Oly");
    assert(t2.m.age == 1989);
}