#pragma once

#include "json_value.hpp"

namespace pson {

template <typename T, bool is_json_type>
struct enable_if
{
    void ObjectInsert(pson::Object& o, std::string name, T t) {}
};

template <typename T>
struct enable_if<T, true>
{
    void ObjectInsert(pson::Object& o, std::string name, T t)
    {
        o.insert(std::move(name), t);
    }
};

template <typename T>
struct enable_if<T, false>
{
    void ObjectInsert(pson::Object& o, std::string name, T t)
    {
        auto&& json = t.Serialize();
        o.insert(std::move(name), json);
    }
};

}

#define VALUE_NAME(a) (#a)

#define Serialize_Start inline pson::Value Serialize() {\
                                            pson::Value v{pson::JSON_OBJECT};\
                                            pson::Object& obj = v.as<pson::Object>();

#define Serialize_Insert(a) pson::enable_if<decltype(a), IS_JSON_TYPE(decltype(a))>().ObjectInsert(obj, std::string(VALUE_NAME(a)) , a);



#define Serialize_End \
                return v; }                    


