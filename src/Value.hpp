//
// Created by pink on 2022/2/26.
//

#ifndef PNET_PSON_VALUE_HPP
#define PNET_PSON_VALUE_HPP

#include <string.h>
#include <assert.h>

#include <map>
#include <string>
#include <vector>

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

    class JsonBadConversion : std::exception
    {};


    class Value;
    class Object;
    class Array;

    typedef std::string String;
    typedef double Number;
    typedef int    Null;
    typedef bool   Bool;

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

    class Object {
    private:

        std::map<std::string, Value*> value_map_;

    public:

        Object();
        ~Object();

        inline void   Insert(std::string name, Value* value);
        inline bool   Has(const std::string& name);
        inline Value& GetValue(const std::string& name);

        inline void reset();

        inline Null& GetAsNull(const String& name);
        inline Bool& GetAsBool(const String& name);
        inline Number& GetAsNumber(const String& name);
        inline String& GetAsString(const String& name);
        inline Array& GetAsArray(const String& name);
        inline Object& GetAsObject(const String& name);
    };

    class Array {
    private:
        std::vector<Value*> values_;
    public:
        Array();

        ~Array();

        inline bool     Has(size_t i);
        inline Value&   GetValue(size_t i);
        inline void     Push(Value* val);
        inline size_t   Size() const;
        inline void     reset();

        inline Null& GetAsNull(size_t i);
        inline Bool& GetAsBool(size_t i);
        inline Number& GetAsNumber(size_t i);
        inline String& GetAsString(size_t i);
        inline Array& GetAsArray(size_t i);
        inline Object& GetAsObject(size_t i);
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

        inline bool JudgeType(const JSON_TYPE& type) { return type == type_; }

    public:

        Value(): type_(JSON_UNKNOW)
        {}

        Value(const JSON_TYPE& type);

        ~Value();

        void reset();

        bool IsNull() { return JudgeType(JSON_NULL); }
        bool IsBool() { return JudgeType(JSON_BOOL); }
        bool IsNumber() { return JudgeType(JSON_NUMBER); }
        bool IsString() { return JudgeType(JSON_STRING); }
        bool IsArray() { return JudgeType(JSON_ARRAY); }
        bool IsObject() { return JudgeType(JSON_OBJECT); }

        inline int& AsNull();
        inline bool& AsBool();
        inline Number& AsNumber();
        inline String& AsString();
        inline Array& AsArray();
        inline Object& AsObject();

        inline void ImportNull();
        inline void ImportBool(bool to);
        inline void ImportNumber(Number num);
        inline void ImportString(String str);
        inline void ImportArray(Array array);
        inline void ImportObject(Object obj);

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

    Value::~Value()
    {
        reset();
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

    int& Value::AsNull()
    {
        if (!IsNull()) throw JsonBadConversion();
        return *CAST(int);
    }

    bool& Value::AsBool()
    {
        if (!IsBool()) throw JsonBadConversion();
        return *CAST(bool);
    }

    Number& Value::AsNumber()
    {
        if (!IsNumber()) throw JsonBadConversion();
        return *CAST(Number);
    }

    String& Value::AsString()
    {
        if (!IsString()) throw JsonBadConversion();
        return *CAST(String);
    }

    Array& Value::AsArray()
    {
        if (!IsArray()) throw JsonBadConversion();
        return *CAST(Array);
    }

    Object& Value::AsObject()
    {
        if (!IsObject()) throw JsonBadConversion();
        return *CAST(Object);
    }

    void Value::ImportNull()
    {
        reset();
        new (buffer_)int(0);
        type_ = JSON_NULL;
    }

    void Value::ImportBool(bool to)
    {
        reset();
        new (buffer_)bool(to);
        type_ = JSON_BOOL;
    }

    void Value::ImportNumber(Number num)
    {
        reset();
        new (buffer_)Number(num);
        type_ = JSON_NUMBER;
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

    Array::Array()
    {}

    Array::~Array()
    {
        reset();
    }

    size_t Array::Size() const
    {
        return values_.size();
    }

    Value& Array::GetValue(size_t i) {
        return *values_[i];
    }

    void Array::Push(Value* val)
    {
        values_.emplace_back(val);
    }

    bool Array::Has(size_t i)
    {
        return values_.size() > i;
    }

    void Array::reset()
    {
        for(auto iter = values_.begin(); iter != values_.end(); ++iter)
        {
            delete *iter;
        }
        values_.clear();
    }

    Null& Array::GetAsNull(size_t i)
    {
        return GetValue(i).AsNull();
    }

    Bool& Array::GetAsBool(size_t i)
    {
        return GetValue(i).AsBool();
    }

    Number& Array::GetAsNumber(size_t i)
    {
        return GetValue(i).AsNumber();
    }

    String& Array::GetAsString(size_t i)
    {
        return GetValue(i).AsString();
    }

    Array& Array::GetAsArray(size_t i)
    {
        return GetValue(i).AsArray();
    }

    Object& Array::GetAsObject(size_t i)
    {
        return GetValue(i).AsObject();
    }

    Object::Object()
    {}

    Object::~Object()
    {
        reset();
    }

    void Object::reset()
    {
        for(auto iter = value_map_.begin(); iter != value_map_.end(); ++iter)
        {
            delete (*iter).second;
        }
        value_map_.clear();
    }

    bool Object::Has(const std::string &name)
    {
        return value_map_.find(name) != value_map_.end();
    }

    void Object::Insert(std::string name, Value *value)
    {
        value_map_.emplace(std::move(name), value);
    }

    Value& Object::GetValue(const std::string &name)
    {
        return *value_map_[name];
    }

    Null& Object::GetAsNull(const String &name)
    {
        return GetValue(name).AsNull();
    }

    Bool& Object::GetAsBool(const String &name)
    {
        return GetValue(name).AsBool();
    }

    Number& Object::GetAsNumber(const String& name)
    {
        return GetValue(name).AsNumber();
    }

    String& Object::GetAsString(const String &name)
    {
        return GetValue(name).AsString();
    }

    Array& Object::GetAsArray(const String &name)
    {
        return GetValue(name).AsArray();
    }

    Object& Object::GetAsObject(const String &name)
    {
        return GetValue(name).AsObject();
    }

#undef CAST
} // namespace pson
#endif
