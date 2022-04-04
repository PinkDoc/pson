//
// Created by pink on 2022/2/26.
//

#ifndef PSON_VALUE_HPP
#define PSON_VALUE_HPP

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

        void   Insert(std::string name, Value* value);
        void   Erase(const std::string& name);
        bool   Has(const std::string& name);
        Value& GetValue(const std::string& name);

        void reset();

        Null& GetAsNull(const String& name);
        Bool& GetAsBool(const String& name);
        Number& GetAsNumber(const String& name);
        String& GetAsString(const String& name);
        Array& GetAsArray(const String& name);
        Object& GetAsObject(const String& name);

        std::map<std::string, Value*>& map() { return value_map_; }
    };

    class Array {
    private:
        std::vector<Value*> values_;
    public:
        Array();

        ~Array();

        bool     Has(size_t i);
        Value&   GetValue(size_t i);
        void     Push(Value* val);
        void     Pop();
        size_t   Size() const;
        void     reset();

        Null& GetAsNull(size_t i);
        Bool& GetAsBool(size_t i);
        Number& GetAsNumber(size_t i);
        String& GetAsString(size_t i);
        Array& GetAsArray(size_t i);
        Object& GetAsObject(size_t i);

        std::vector<Value*>& values() { return values_; }
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

        JSON_TYPE type() const { return type_; }

        bool IsNull() { return JudgeType(JSON_NULL); }
        bool IsBool() { return JudgeType(JSON_BOOL); }
        bool IsNumber() { return JudgeType(JSON_NUMBER); }
        bool IsString() { return JudgeType(JSON_STRING); }
        bool IsArray() { return JudgeType(JSON_ARRAY); }
        bool IsObject() { return JudgeType(JSON_OBJECT); }

        int& AsNull();
        bool& AsBool();
        Number& AsNumber();
        String& AsString();
        Array& AsArray();
        Object& AsObject();

        void ImportNull();
        void ImportBool(bool to);
        void ImportNumber(Number num);
        void ImportString(String str);
        void ImportArray(Array array);
        void ImportObject(Object obj);

    };

    inline Value::Value(const JSON_TYPE &type) {
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

    inline Value::~Value()
    {
        reset();
    }

    inline void Value::reset() {
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

    inline int& Value::AsNull()
    {
        if (!IsNull()) throw JsonBadConversion();
        return *CAST(int);
    }

    inline bool& Value::AsBool()
    {
        if (!IsBool()) throw JsonBadConversion();
        return *CAST(bool);
    }

    inline Number& Value::AsNumber()
    {
        if (!IsNumber()) throw JsonBadConversion();
        return *CAST(Number);
    }

    inline String& Value::AsString()
    {
        if (!IsString()) throw JsonBadConversion();
        return *CAST(String);
    }

    inline Array& Value::AsArray()
    {
        if (!IsArray()) throw JsonBadConversion();
        return *CAST(Array);
    }

    inline Object& Value::AsObject()
    {
        if (!IsObject()) throw JsonBadConversion();
        return *CAST(Object);
    }

    inline void Value::ImportNull()
    {
        reset();
        new (buffer_)int(0);
        type_ = JSON_NULL;
    }

    inline void Value::ImportBool(bool to)
    {
        reset();
        new (buffer_)bool(to);
        type_ = JSON_BOOL;
    }

    inline void Value::ImportNumber(Number num)
    {
        reset();
        new (buffer_)Number(num);
        type_ = JSON_NUMBER;
    }

    inline void Value::ImportString(String str)
    {
        reset();
        new (buffer_)String(::std::move(str));
        type_ = JSON_STRING;
    }

    inline void Value::ImportArray(Array array)
    {
        reset();
        new (buffer_)Array(::std::move(array));
        type_ = JSON_ARRAY;
    }

    inline void Value::ImportObject(Object obj)
    {
        reset();
        new (buffer_)Object(::std::move(obj));
        type_ = JSON_OBJECT;
    }

    inline Array::Array()
    {}

    inline Array::~Array()
    {
        reset();
    }

    inline size_t Array::Size() const
    {
        return values_.size();
    }

    inline Value& Array::GetValue(size_t i) 
    {
        return *values_[i];
    }

    inline void Array::Push(Value* val)
    {
        values_.emplace_back(val);
    }

    inline void Array::Pop()
    {
        auto v = values_.back();
        values_.pop();
        delete v;
    }

    inline bool Array::Has(size_t i)
    {
        return values_.size() > i;
    }

    inline void Array::reset()
    {
        for(auto iter = values_.begin(); iter != values_.end(); ++iter)
        {
            delete *iter;
        }
        values_.clear();
    }

    inline Null& Array::GetAsNull(size_t i)
    {
        return GetValue(i).AsNull();
    }

    inline Bool& Array::GetAsBool(size_t i)
    {
        return GetValue(i).AsBool();
    }

    inline Number& Array::GetAsNumber(size_t i)
    {
        return GetValue(i).AsNumber();
    }

    inline String& Array::GetAsString(size_t i)
    {
        return GetValue(i).AsString();
    }

    inline Array& Array::GetAsArray(size_t i)
    {
        return GetValue(i).AsArray();
    }

    inline Object& Array::GetAsObject(size_t i)
    {
        return GetValue(i).AsObject();
    }

    inline Object::Object()
    {}

    inline Object::~Object()
    {
        reset();
    }

    inline void Object::reset()
    {
        for(auto iter = value_map_.begin(); iter != value_map_.end(); ++iter)
        {
            delete (*iter).second;
        }
        value_map_.clear();
    }

    inline bool Object::Has(const std::string &name)
    {
        return value_map_.find(name) != value_map_.end();
    }

    inline void Object::Insert(std::string name, Value *value)
    {
        if ((auto iter = value_map_.find(name)) != value_map_.end()) {
            delete *iter.second;
            value_map_.erase(name);
        }
        value_map_.emplace(std::move(name), value);
    }

    inline void Object::Erase(const std::string& name)
    {
        if ((auto iter = value_map_.find(name)) != value_map_.end()) {
            delete *iter.second;
            value_map_.erase(name);
        }
    }

    inline Value& Object::GetValue(const std::string &name)
    {
        return *value_map_[name];
    }

    inline Null& Object::GetAsNull(const String &name)
    {
        return GetValue(name).AsNull();
    }

    inline Bool& Object::GetAsBool(const String &name)
    {
        return GetValue(name).AsBool();
    }

    inline Number& Object::GetAsNumber(const String& name)
    {
        return GetValue(name).AsNumber();
    }

    inline String& Object::GetAsString(const String &name)
    {
        return GetValue(name).AsString();
    }

    inline Array& Object::GetAsArray(const String &name)
    {
        return GetValue(name).AsArray();
    }

    inline Object& Object::GetAsObject(const String &name)
    {
        return GetValue(name).AsObject();
    }

#undef CAST
} // namespace pson
#endif
