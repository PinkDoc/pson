
#pragma once

#include "json_value.hpp"

namespace pson {


#define IS_INTEGER(type) \
    (pson::TypesCmp<type,uint8_t, int8_t, uint16_t, int16_t, int32_t, uint32_t, int64_t, uint64_t>::value)

// This is for json data
template <typename T, bool is_integer>
struct FetchJsonDataT
{
     inline void Fetch(pson::Object& o, const std::string& name, T& t) {}
};

template <typename T>
struct FetchJsonDataT<T, true>
{
    inline void Fetch(pson::Object& o, const std::string& name, T& t) {
        pson::Number n = o.at(name).as<Number>();
        t = static_cast<T>(n);
    }
};

template <typename T>
struct FetchJsonDataT<T, false>
{
    inline void Fetch(pson::Object& o, const std::string& name, T& t) {
        t = o.at(name).as<T>();
    }
};


template <typename T, bool is_json_type>
struct enable_if
{
    inline void ObjectInsert(pson::Object& o, std::string name, T t) {}
    inline void ObjectFetch(pson::Object& o, const std::string& name, T &t) {}
};

template <typename T>
struct enable_if<T, true>
{
    inline void ObjectInsert(pson::Object& o, std::string name, T t)
    {
        o.insert(std::move(name), t);
    }

    inline void ObjectFetch(pson::Object& o, const std::string& name, T& t)
    {
       FetchJsonDataT<T, IS_INTEGER(T)>().Fetch(o, name, t);
    }
};

template <typename T>
struct enable_if<T, false>
{
    inline void ObjectInsert(pson::Object& o, std::string name, T t)
    {
        auto&& json = t.Serialize();
        o.insert(std::move(name), json);
    }

    inline void ObjectFetch(pson::Object& o, const std::string& name, T& t)
    {
        auto&& v = o.at(name);
        t.DeSerializeFromValue(v);
    }
};

}

#define VALUE_NAME(a) (#a)

#define Serialize_Start inline pson::Value Serialize() {\
                                            pson::Value v{pson::JSON_OBJECT};\
                                            pson::Object& obj = v.as<pson::Object>();

#define Serialize_Insert(a) pson::enable_if<decltype(a), IS_JSON_TYPE(decltype(a))>().ObjectInsert(obj, std::move(std::string(VALUE_NAME(a))), a);

#define Serialize_End \
                return v; }        


#define DeSerialize_Start inline void DeSerializeFromValue(pson::Value& v) { \
                                    auto&& obj = v.as<pson::Object>();
                                    
#define DeSerialize_Fetch(a) pson::enable_if<decltype(a), IS_JSON_TYPE(decltype(a))>().ObjectFetch(obj, std::string(VALUE_NAME(a)), a);

#define DeSerialize_End }


