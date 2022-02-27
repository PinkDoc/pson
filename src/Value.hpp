//
// Created by pink on 2022/2/26.
//

#ifndef PNET_PSON_VALUE_HPP
#define PNET_PSON_VALUE_HPP

#include <string>
#include <vector>

#include "Object.hpp"
#include "Array.hpp"

namespace pson {

    class Value;
    class Array;
    class Object;

    typedef std::string String;
    typedef double Number;

    enum JSON_TYPE
    {
        JSON_UNKNOW,
        JSON_NULL,
        JSON_BOOL,
        JSON_NUMBER,
        JSON_STRING,
        JSON_ARRAY,
        JSON_OBJECT
    };


    class Value {
    private:
        template <typename arg, typename... args>
        struct union_size;

        template <typename arg>
        struct union_size<arg> : std::integral_constant<size_t, sizeof(arg)> {};

        template <typename arg0, typename arg1, typename... args>
        struct union_size<arg0, arg1, args...>
                : std::integral_constant<size_t, sizeof(arg0) >= sizeof(arg1)
                                                 ? union_size<arg0, args...>::value
                                                 : union_size<arg1, args...>::value> {
        };

        static constexpr size_t buf_size = union_size<int, bool, Number, String, Array, Object>::value;

        JSON_TYPE type_;
        char buffer_[buf_size];

        #define CAST(type) \
            reinterpret_cast<type*>(buffer_)

        bool JudgeType(const JSON_TYPE& type) { return type == type_; }

    public:

        explicit
        Value(): type_(JSON_UNKNOW)
        {}

        explicit
        Value(const JSON_TYPE& type);

        ~Value() { reset(); }

        void reset();

        bool IsNull() { JudgeType(JSON_NULL); }
        bool IsBool() { JudgeType(JSON_BOOL); }
        bool IsNumber() { JudgeType(JSON_NUMBER); }
        bool IsArray() { JudgeType(JSON_ARRAY); }
        bool IsObject() { JudgeType(JSON_OBJECT); }

        int& AsNull() { reset(); type_ = JSON_NULL; return *CAST(int); }
        bool& AsBool() { reset(); type_ == JSON_BOOL; return *CAST(bool); }
        Number& AsNumber() { reset(); type_ == JSON_NUMBER; return *CAST(Number); }
        String& AsString() { reset(); type_ == JSON_STRING; return *CAST(String); }
        Array& AsArray() { reset(); type_ == JSON_ARRAY; return *CAST(Array); }
        Object& AsObject() { reset(); type_ == JSON_OBJECT; return *CAST(Object); }
    };

    Value::Value(const JSON_TYPE &type) {
        switch (type) {
            case JSON_NULL:
            {
                new (buffer_)int(0);
                break;
            }
            case JSON_BOOL:
            {
                new (buffer_)bool(false);
                break;
            }
            case JSON_NUMBER:
            {
                new (buffer_)Number(0);
                break;
            }
            case JSON_STRING:
            {
                new (buffer_)String();
                break;
            }
            case JSON_ARRAY:
            {
                new (buffer_)Array();
                break;
            }
            case JSON_OBJECT:
            {
                new (buffer_)Object();
                break;
            }
        }
    }

    void Value::reset() {
        if (type_ == JSON_STRING)
        {
            auto p = CAST(String);
            p->String::~String();
        }
        if (type_ == JSON_ARRAY)
        {
            auto p = CAST(Array);
            p->Array::~Array();
        }
        if (type_ == JSON_OBJECT)
        {
            auto p = CAST(Object);
            p->Object::~Object();
        }
        type_ = JSON_UNKNOW;
    }

} // namespace pson
#endif
