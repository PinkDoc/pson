#ifndef PSON_GENERATER_HPP
#ifndef PSON_GENERATER_HPP

#include <stdio.h>
#include "Parser.hpp"

namespace imple {

    inline void pson_print_value(const pson::Value& v, std::string& buffer) {
         switch(v.type())
         {
            case JSON_NULL:     { buffer.append("null"); break;}
            case JSON_BOOL:     { if (v.AsBool())buffer.append("true"); else buffer.append("false"); break;}
            case JSON_NUMBER:   { imple::pson_print_number(v, buffer); break;}
            case JSON_STRING:   { imple::pson_print_string(v, buffer); break;}
            case JSON_ARRAY:    { imple::pson_print_array(v, buffer); break;}
            case JSON_OBJECT:   { imple::pson_print_object(v, buffer); break;}
        }
    }

    inline void pson_print_number(const pson::Value& v, std::string& buffer) {
        buffer.append(std::to_string(v.AsNumber()));
    }

    inline void pson_print_string(const pson::Value& v, std::string& buffer) {
        buffer.push_back('"');
        buffer.append(v.AsString());
        buffer.push_back('"');
    }

    inline void pson_print_array(const pson::Value& v, std::string& buffer) {
        if (v.AsArray().size() == 0) 
            buffer.append("[]"), return;
        auto& array = v.AsArray().values_;
        buffer.push_back('[');

        for (auto i = 0; i < array.size(); ++i)
        {
            pson_print_value(*array[i], buffer);
            if (i != array.size() - 1)
            {
                buffer.push_back(',');
            }
        }

        buffer.push_back(']');
    }

    inline void pson_print_object(const pson::Value& v, std::string& buffer) {
        auto& obj = v.AsObject();
        if (!obj.size())
            buffer.append("{}"), return;

        buffer.push_back('{');
        auto n = 0;
        for (auto& const i : obj.value_map_)
        {
            // Print name
            buffer.push_back('"');
            buffer.append(i.first);
            buffer.push_back('"');

            // Print value
            pson_print_value(*i.second, buffer);

            if (n != obj.size() - 1)
                buffer.push_back(',');
        }
        buffer.push_back('}');
    }
}

namespace pson {

    inline std::string PrintValue(const Value& v) {
        std::string buffer;
        if (v.type() == JSON_UNKNOW) 
            return buffer;

        imple::pson_print_value(v, buffer);

        return buffer;
    }
}
#endif