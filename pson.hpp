#ifndef PINK_JSON_H
#define PINK_JSON_H

#include <map>
#include <assert.h>
#include <vector>
#include <string>

// RFC 4627 
// Allow write space:  ws = *(
//                                  %x20 /              ; Space
//                                  %x09 /              ; Horizontal tab
//                                  %x0A /              ; Line feed or New line
//                                  %x0D                ; Carriage return
//                              )
// JSON text SHALL be encoded in Unicode.  The default encoding is
// UTF-8.

namespace pson {

    class Array;
    class Object;
    class Value;

    typedef std::string String;
    typedef bool Bool;
    typedef double Number;
    typedef int Null;

    enum JSON_TYPE {
        JSON_UNKNOW,
        JSON_STRING,
        JSON_ARRAY,
        JSON_OBJECT,
        JSON_NUMBER,
        JSON_BOOL,
        JSON_NULL
    };
    
    class Object {
    private:
        typedef std::map<String, Value*> Contaioner;
    public:
        Contaioner value_map_;

        template<typename T>
        T& Get();

        template<typename T>
        bool Has(const String& name);
    };

    class Array {
    private:
        typedef std::vector<Value*> Contaioner;
    public:
        Contaioner values_;
    };

    // A JSON value MUST be an object, array, number, or string, or one of
    // the following three literal names: false true null
    class Value {
    private:

        template <typename arg, typename ... args>
        struct union_size;

        template <typename arg>
        struct union_size<arg> : std::integral_constant<size_t, sizeof(arg)> {};

        template <typename arg0, typename arg1, typename... args>
        struct union_size<arg0, arg1, args...> : std::integral_constant < size_t,
                sizeof(arg0) >= sizeof(arg1) ? union_size<arg0, args...>::value
                                             : union_size<arg1, args...>::value > {};

        static constexpr size_t data_size = union_size<Number, Null, Bool, Array, Object, String>::value;

        JSON_TYPE type_;
        char buffer[data_size];

    public:
        Value()
        Value(const JSON_TYPE& type);
        ~Value();

        void reset();

        template<typname T>
        void Change(T& value);

        template<typename T>
        T& GetValue();

        template<typename T>
        bool
    };

    Value::Value(): type_(JSON_UNKNOW) {}

    Value::Value(const JSON_TYPE& type):
        type_(type)
    {
        switch (type) {
            case JSON_STRING: {
                 new (buffer)String();
                break;
            }
            case JSON_ARRAY: {
                new (buffer)Array();
                break;
            }
            case JSON_OBJECT: {
                new (buffer)Object();
                break;
            }
            case JSON_NULL: {
                new (buffer)Null(0);
            }
            case JSON_BOOL: {
                new(buffer)Bool();
            }
            case JSON_NUMBER: {
                new(buffer)Number();
            }
        }
        type_ = type;
    }

    void Value::reset() {
        if (type_ == JSON_STRING) {
            auto p = reinterpret_cast<String*>(buffer);
            p->String::~String();
        } else if (type_ == JSON_ARRAY) {
            auto p = reinterpret_cast<Array*>(buffer);
            p->Array::~Array();
        } else if (type_ == JSON_OBJECT) {
            auto p = reinterpret_cast<Object*>(buffer);
            p->Object::~Object();
        }
    }

    Value::~Value()
    { reset(); }


    template <>
    String& Value::GetValue<String>()
    {
        return *reinterpret_cast<String*>(buffer);
    }

    template <>
    Array& Value::GetValue<Array>()
    {
        return *reinterpret_cast<Array*>(buffer);
    }

    template <>
    Object& Value::GetValue<Object>()
    {
        return *reinterpret_cast<Object*>(buffer);
    }

    template <>
    Number& Value::GetValue<Number>()
    {
        return *reinterpret_cast<Number*>(buffer);
    }

    template <>
    Bool& Value::GetValue<Bool>()
    {
        return *reinterpret_cast<Bool*>(buffer);
    }

    template <>
    Null& Value::GetValue<Null>()
    {
        return *reinterpret_cast<Bool*>(buffer);
    }

}   // namespace pson

#endif