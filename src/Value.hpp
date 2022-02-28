//
// Created by pink on 2022/2/26.
//

#ifndef PNET_PSON_VALUE_HPP
#define PNET_PSON_VALUE_HPP

#include <string.h>
#include <assert.h>

#include <string>
#include <vector>

#include "Object.hpp"
#include "Array.hpp"

#define PSON_ASSERT(res) \
    pson::assertion(__FILE__, __LINE__, res)


namespace pson {

    static void assertion(char* filename, int line, bool res)
    {
        if (!res)
        {
            fprintf(stderr, "[PSON ERROR] : file %s at line %d \n", filename, line);
            assert(false);
        }
    }

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

        explicit
        Value(Value&& val);

        explicit
        Value(const Value& val);

        ~Value() { reset(); }

        void reset();

        bool IsNull() { return JudgeType(JSON_NULL); }
        bool IsBool() { return JudgeType(JSON_BOOL); }
        bool IsNumber() { return JudgeType(JSON_NUMBER); }
        bool IsArray() { return JudgeType(JSON_ARRAY); }
        bool IsObject() { return JudgeType(JSON_OBJECT); }

        int& AsNull() { PSON_ASSERT(type_ == JSON_NULL); return *CAST(int); }
        bool& AsBool() { PSON_ASSERT(type_ == JSON_BOOL);  return *CAST(bool); }
        Number& AsNumber() { PSON_ASSERT(type_ == JSON_NUMBER); return *CAST(Number); }
        String& AsString() { PSON_ASSERT(type_ = JSON_STRING); return *CAST(String); }
        Array& AsArray() { PSON_ASSERT(type_ == JSON_ARRAY); return *CAST(Array); }
        Object& AsObject() { PSON_ASSERT(type_ == JSON_OBJECT); return *CAST(Object); }

        void ImportNull() { reset(); type_ = JSON_NULL; *CAST(int) = 0; }

        void ImportBool(bool to) { reset(); type_ = JSON_BOOL; *CAST(bool) = to; }

        void ImportNumber(Number num) { reset(); type_ = JSON_NUMBER; *CAST(Number) = num; }

        void ImportString(String str);

        void ImportArray(Array array);

        void ImportObject(Object obj);
    };

    Value::Value(const JSON_TYPE &type) {
        type_ = type;
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

    Value::Value(Value&& val)
    {
#define AS(type) *reinterpret_cast<type*>(val.buffer_)
        switch (val.type_) {
            case JSON_NULL:
            {
                new (buffer_)int(0);
                break;
            }
            case JSON_BOOL:
            {
                new (buffer_)bool(AS(bool));
                break;
            }
            case JSON_NUMBER:
            {
                new (buffer_)Number(AS(Numebr));
                break;
            }
            case JSON_STRING:
            {
                new (buffer_)String(std::move(AS(String)));
                break;
            }
            case JSON_ARRAY:
            {
                new (buffer_)Array(std::move(AS(Array)));
                break;
            }
            case JSON_OBJECT:
            {
                new (buffer_)Object(std::move(AS(Object)));
                break;
            }
        }
        type_ = val.type_;
#undef AS;
    }


    Value(const Value& val)
    {
#define AS(type) *reinterpret_cast<type*>(val.buffer_)

        switch(val.type_)
        {
            case JSON_NULL:
            {
                new (buffer_)int(0);
                break;
            }
            case JSON_BOOL:
            {
                new (buffer_)bool(AS(bool));
                break;
            }
            case JSON_NUMBER:
            {
                new (buffer_)Number(AS(Number));
                break;
            }
            case JSON_STRING:
            {
                new (buffer_)String(AS(String));
                break;
            }
            case JSON_ARRAY:
            {
                new (buffer_)Array(AS(Array));
                break;
            }
            case JSON_OBJECT:
            {
                new (buffer_)Object(AS(Object));
                break;
            }
        }
        type_ = val.type_;
#undef AS
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

    void Value::ImportString(String str)
    {
        reset();
        new (buffer_)String(::std::move(str));
        type_ = JSON_STRING;
    }

    void Value::ImportArray(Array array)
    {
        reset();
        new (buffer_)Array(::std::move(array));
        type_ = JSON_ARRAY;
    }

    void Value::ImportObject(Object obj)
    {
        reset();
        new (buffer_)Object(::std::move(obj));
        type_ = JSON_OBJECT;
    }

} // namespace pson
#endif
