#pragma once

#include <string>
#include <vector>
#include <map>

#include <string.h>

#include "json_assert.hpp"

namespace pson{

    // Json type
    enum JSON_TYPE
    {
        JSON_NULL,
        JSON_BOOL,
        JSON_NUMBER,
        JSON_STRING,
        JSON_ARRAY,
        JSON_OBJECT
    };

    class Value;

    using Null   = short;
    using Bool   = bool;
    using Number = double;
    using String = std::string;
    using ArrayContainer = std::vector<Value>;
    using ObjectContainer = std::map<std::string, Value>;

    class Object {
    private:
        ObjectContainer value_map_;
    public:

        Object() = default;
        ~Object() = default;

        Object(const Object& o);

        Object(Object&& o);

        Object& operator= (const Object& o);
        Object& operator= (Object&& o);

        // You could use move to zero over head,
        // eg. obj.insert(std::move(name), std::move(val));
        // Copy eg. obj.insert(name, val);
        void    insert(std::string name, Value v);
        void    erase(const std::string& name);
        Value&  operator[] (const std::string& name);
        Value&  at(const std::string& name);

        bool        empty()                         const;
        bool        has(const std::string& name)    const ;
        std::size_t size()                          const;
    };


    class Array {
    private:
        ArrayContainer values_;
    public:

        Array() = default;
        ~Array() = default;

        Array(const Array &a);
        Array(Array &&a);

        Array &operator=(const Array &a);

        Array &operator=(Array &&a);

        Value& operator[] (int p);
        void    push_back(Value v);
        void    pop_back();
        Value&  back();
        Value&  at(int i);

        bool        has(int i)          const;
        std::size_t size()              const;
        bool        empty()             const;
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
        char val_[buf_size];


        template<typename T>
        const T& const_as() const;

        bool judge_type(const JSON_TYPE& t) const { return type_ == t; }

        void init_base(const JSON_TYPE& t);

        void swap_value(Value& v) {
            char buf[buf_size];
            memcpy(buf, val_, buf_size);
            memcpy(val_, v.val_, buf_size);
            memcpy(v.val_, buf, buf_size);
            std::swap(type_, v.type_);
        }

        void copy_value(const Value& v);

    public:

        Value();

        Value(const JSON_TYPE&);

        // Copy
        Value(const Value&);

        // Move
        Value(Value&&);

        ~Value();

        // Copy
        Value& operator= (const Value&);

        // Move
        Value& operator= (Value&&);

        void reset();

        void reset_as(JSON_TYPE t) { reset(); init_base(t); }

        template <typename T>
        void reset_as();

        void swap(Value& v) { swap_value(v); }

        bool is_null()      const { return judge_type(JSON_NULL); }
        bool is_bool()      const { return judge_type(JSON_BOOL); }
        bool is_number()    const { return judge_type(JSON_NUMBER); }
        bool is_string()    const { return judge_type(JSON_STRING); }
        bool is_array()     const { return judge_type(JSON_ARRAY); }
        bool is_object()    const { return judge_type(JSON_OBJECT); }

        template <typename T>
        bool is();

        Null as_null() {
            PSON_ASSERT(judge_type(JSON_NULL));
            return *reinterpret_cast<Null*>(val_);
        }

        Number& as_number() {
            PSON_ASSERT(judge_type(JSON_NUMBER));
            return *reinterpret_cast<Number*>(val_);
        }

        Bool& as_bool() {
            PSON_ASSERT(judge_type(JSON_BOOL));
            return *reinterpret_cast<Bool*>(val_);
        }

        String& as_string() {
            PSON_ASSERT(judge_type(JSON_STRING));
            return *reinterpret_cast<String*>(val_);
        }

        Array&  as_array() {
            PSON_ASSERT(judge_type(JSON_ARRAY));
            return *reinterpret_cast<Array*>(val_);
        }

        Object& as_object() {
            PSON_ASSERT(judge_type(JSON_OBJECT));
            return *reinterpret_cast<Object*>(val_);
        }

        template<typename T>
        T& as();

        bool empty() const;
        std::size_t size() const;       // For array, object

        // operator for Array
        void    push_back(Value v);
        void    pop_back();
        Value&  back();
        bool    has(int i) const;
        Value&  operator[] (int i);
        Value&  at(int i);

        // operator for Object
        void    insert(std::string name, Value v);
        bool    has(const std::string& name) const;
        Value&  operator[] (const std::string& name);
        Value&  at(const std::string& name);
        void    erase(const std::string& name);

        // parse
        bool parse(const std::string &json_data);
        bool parse(char *json_data, std::size_t len);
    };



    // Implement
    // Object

    inline Object::Object(const Object& o):
        value_map_(o.value_map_)
    {}

    inline Object::Object(Object&& o):
        value_map_(std::move(o.value_map_))
    {}

    inline Object& Object::operator= (const Object& o)
    {
        value_map_ = o.value_map_;
        return *this;
    }

    inline Object& Object::operator= (Object&& o)
    {
        value_map_ = std::move(o.value_map_);
        return *this;
    }

    inline Value &Object::operator[](const std::string &name)
    {
        return value_map_.at(name);
    }

    inline bool Object::empty() const
    {
        return value_map_.empty();
    }

    inline Value& Object::at(const std::string& name)
    {
        return value_map_.at(name);
    }

    inline std::size_t Object::size() const 
    {
        return value_map_.size();
    }

    inline bool Object::has(const std::string &name) const
    {
        return value_map_.find(name) != value_map_.end();
    }

    inline void Object::insert(std::string name, Value v)
    {
        value_map_.emplace(std::move(name), std::move(v));
    }

    inline void Object::erase(const std::string& name)
    {
        value_map_.erase(name);
    }

    // Array

    inline Array::Array(const Array& a):
        values_(a.values_)
    {}

    inline Array::Array(Array&& a):
        values_(std::move(a.values_))
    {}

    inline Array& Array::operator= (const Array& a)
    {
        values_ = a.values_;
        return *this;
    }

    inline Array& Array::operator= (Array&& a)
    {
        values_ = std::move(a.values_);
        return *this;
    }

    inline bool Array::empty() const
    {
        return values_.empty();
    }

    inline Value &Array::back()
    {
        return values_.back();
    }

    inline Value &Array::at(int i)
    {
        return values_[i];
    }

    inline bool Array::has(int i) const
    {
        return i >= 0 && values_.size() > i;
    }

    inline std::size_t Array::size() const 
    {
        return values_.size();
    }

    inline void Array::push_back(Value v)
    {
        return values_.emplace_back(std::move(v));
    }

    inline void Array::pop_back()
    {
        values_.pop_back();
    }

    inline Value &Array::operator[](int p)
    {
        return values_[p];
    }

    inline Value& Value::back()
    {
        PSON_ASSERT(judge_type(JSON_ARRAY));
        return reinterpret_cast<Array*>(val_)->back();
    }

    inline void Value::pop_back()
    {
        PSON_ASSERT(judge_type(JSON_ARRAY));
        reinterpret_cast<Array*>(val_)->pop_back();
    }

    // Value
    inline Value::Value()
    {
        type_ = JSON_NULL;
        *reinterpret_cast<Null*>(val_) = 0;
    }

    inline Value::Value(const JSON_TYPE &t) 
    {
        init_base(t);
    }

    inline Value::Value(const Value& v)
    {
        copy_value(v);
    }

    inline Value::Value(Value&& other) 
    {
        init_base(JSON_NULL);
        swap_value(other);
    }

    inline Value& Value::operator= (const Value& v) 
    {
        reset();
        copy_value(v);
        return *this;
    }

    inline Value& Value::operator= (Value&& v)
    {
        reset();
        swap_value(v);
        return *this;
    }

    inline Value::~Value()
    {
        switch (type_) 
        {
            case JSON_STRING:
                reinterpret_cast<String*>(val_)->String::~String();
                break;
            case JSON_ARRAY:
                reinterpret_cast<Array*>(val_)->Array::~Array();
                break;
            case JSON_OBJECT:
                reinterpret_cast<Object*>(val_)->Object::~Object();
                break;
        }
    }

    inline void Value::reset() 
    {
        switch (type_) 
        {
            case JSON_STRING:
                reinterpret_cast<String*>(val_)->String::~String();
                break;
            case JSON_ARRAY:
                reinterpret_cast<Array*>(val_)->Array::~Array();
                break;
            case JSON_OBJECT:
                reinterpret_cast<Object*>(val_)->Object::~Object();
                break;
        }
        type_ = JSON_NULL;
        *reinterpret_cast<Null*>(val_) = 0;
    }

    inline void Value::init_base(const JSON_TYPE& t) 
    {
        type_ = t;
        switch (type_) 
        {
            case JSON_NULL:
                *reinterpret_cast<Null*>(val_) = 0;
                break;
            case JSON_BOOL:
                *reinterpret_cast<Bool*>(val_) = false;
                break;
            case JSON_NUMBER:
                *reinterpret_cast<Number*>(val_) = 0.0;
                break;
            case JSON_STRING:
                new (val_)String();
                break;
            case JSON_ARRAY:
                new (val_)Array();
                break;
            case JSON_OBJECT:
                new (val_)Object();
                break;
        }
    }

    inline void Value::copy_value(const Value& v) 
    {
        type_ = v.type_;
        switch (v.type_)
        {
            case JSON_NULL:
                *reinterpret_cast<Null*>(val_) = 0;
                break;
            case JSON_BOOL:
                *reinterpret_cast<Bool*>(val_) = v.const_as<Bool>();
                break;
            case JSON_NUMBER:
                *reinterpret_cast<Number*>(val_) = v.const_as<Number>();
                break;
            case JSON_STRING:
                 new (val_)String(v.const_as<String>());
                break;
            case JSON_ARRAY:
                new (val_)Array(v.const_as<Array>());
                break;
            case JSON_OBJECT:
                new (val_)Object(v.const_as<Object>());
                break;
        }
    }

    // void Value::reset_as<T>()
    template <>
    inline void Value::reset_as<Null>()
    {
        reset_as(JSON_NULL);
    }

    template <>
    inline void Value::reset_as<Bool>()
    {
        reset_as(JSON_BOOL);
    }

    template <>
    inline void Value::reset_as<Number>()
    {
        reset_as(JSON_NUMBER);
    }

    template <>
    inline void Value::reset_as<String>()
    {
        reset_as(JSON_STRING);
    }

    template <>
    inline void Value::reset_as<Array>()
    {
        reset_as(JSON_ARRAY);
    }

    template <>
    inline void Value::reset_as<Object>()
    {
        reset_as(JSON_OBJECT);
    }

    // bool Value::is<T>()
    template <>
    inline bool Value::is<Null>()
    {
        return judge_type(JSON_NULL);
    }

    template <>
    inline bool Value::is<Bool>()
    {
        return judge_type(JSON_BOOL);
    }

    template <>
    inline bool Value::is<Number>()
    {
        return judge_type(JSON_NUMBER);
    }

    template <>
    inline bool Value::is<String>()
    {
        return judge_type(JSON_STRING);
    }

    template <>
    inline bool Value::is<Array>()
    {
        return judge_type(JSON_ARRAY);
    }

    template <>
    inline bool Value::is<Object>()
    {
        return judge_type(JSON_OBJECT);
    }

    // T& Value::as<T>()
    template <>
    inline Null& Value::as<Null>()
    {
        PSON_ASSERT(judge_type(JSON_NULL));
        return *reinterpret_cast<Null*>(val_);
    }

    template <>
    inline Bool& Value::as<Bool>()
    {
        PSON_ASSERT(judge_type(JSON_BOOL));
        return *reinterpret_cast<Bool*>(val_);
    }

    template <>
    inline Number& Value::as<Number>()
    {
        PSON_ASSERT(judge_type(JSON_NUMBER));
        return *reinterpret_cast<Number*>(val_);
    }

    template <>
    inline String& Value::as<String>()
    {
        PSON_ASSERT(judge_type(JSON_STRING));
        return *reinterpret_cast<String*>(val_);
    }

    template <>
    inline Array& Value::as<Array>()
    {
        PSON_ASSERT(judge_type(JSON_ARRAY));
        return *reinterpret_cast<Array*>(val_);
    }

    template <>
    inline Object& Value::as<Object>()
    {
        PSON_ASSERT(judge_type(JSON_OBJECT));
        return *reinterpret_cast<Object*>(val_);
    }

    inline bool Value::empty() const 
    {
        switch (type_)
        {
            case JSON_NULL:
                return true;
            case JSON_ARRAY:
                return const_as<Array>().empty();
            case JSON_OBJECT:
                return const_as<Object>().empty();
            default:
                PSON_ASSERT(false);
        }
        return false;   // Make happy
    }

    inline std::size_t Value::size() const
    {
        switch(type_)
        {
            case JSON_STRING:
                return const_as<String>().size();
            case JSON_ARRAY:
                return const_as<Array>().size();
            case JSON_OBJECT:
                return const_as<Object>().size();
            default:
                PSON_ASSERT(true);
        }
        return 0;
    }

    inline void Value::push_back(Value v)
    {
        PSON_ASSERT(judge_type(JSON_ARRAY));
        as<Array>().push_back(std::move(v));
    }

    inline bool Value::has(int i) const
    {
        PSON_ASSERT(judge_type(JSON_BOOL));
        return const_as<Array>().has(i);
    }

    inline Value& Value::operator[] (int i)
    {
        PSON_ASSERT(judge_type(JSON_ARRAY));
        return reinterpret_cast<Array*>(val_)->at(i);
    }

    inline Value& Value::at(int i)
    {
        PSON_ASSERT(judge_type(JSON_ARRAY));
        return reinterpret_cast<Array*>(val_)->at(i);
    }

    inline void Value::insert(std::string name, Value v)
    {
        PSON_ASSERT(judge_type(JSON_OBJECT));
        return reinterpret_cast<Object*>(val_)->insert(std::move(name), std::move(v));
    }

    inline bool Value::has(const std::string& name) const
    {
        PSON_ASSERT(judge_type(JSON_OBJECT));
        return const_as<Object>().has(name);
    }

    inline Value& Value::operator[] (const std::string& name)
    {
        PSON_ASSERT(judge_type(JSON_OBJECT));
        return as<Object>().at(name);
    }

    inline Value& Value::at(const std::string& name)
    {
        PSON_ASSERT(judge_type(JSON_OBJECT));
        return as<Object>().at(name);
    }

    inline void Value::erase(const std::string &name)
    {
        PSON_ASSERT(judge_type(JSON_OBJECT));
        as<Object>().erase(name);
    }

#undef CAST
#undef PTR_CAST

}