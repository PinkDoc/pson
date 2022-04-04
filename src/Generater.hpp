#ifndef PSON_GENERATER_HPP
#define PSON_GENERATER_HPP

#include <stdio.h>
#include "Parser.hpp"

namespace imple {

    void pson_print_array(pson::Value& v, std::string& buffer);
    void pson_print_number(pson::Value& v, std::string& buffer);
    void pson_print_object(pson::Value& v, std::string& buffer);
    void pson_print_string(pson::Value& v, std::string& buffer);

    inline void pson_print_value(pson::Value& v, std::string& buffer) {
        using namespace pson;
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

    inline void pson_print_number(pson::Value& v, std::string& buffer) {
        buffer.append(std::to_string(v.AsNumber()));
    }

    // FIXME, Support UTF-8...
    inline void pson_print_string(pson::Value& v, std::string& buffer) {
        buffer.push_back('"');
        buffer.append(v.AsString());
        buffer.push_back('"');
    }

    inline void pson_print_array(pson::Value& v, std::string& buffer) {
        if (v.AsArray().Size() == 0) {
            buffer.append("[]");
            return;
        }

        auto& array = v.AsArray().values();
        buffer.push_back('[');

        for (auto i = 0; i < array.size(); ++i)
        {
            pson_print_value(*array[i], buffer);
            if (i != array.size() - 1)
            {
                buffer.append(" ,");
            }
        }

        buffer.push_back(']');
    }

    inline void pson_print_object(pson::Value& v, std::string& buffer) {
        auto& obj = v.AsObject().map();

        if (!obj.size()) {
            buffer.append("{}");
            return;
        }

        buffer.append("{\n");
        auto n = 0;
        for (auto& i : obj)
        {
            // Print name

            buffer.append("\"");
            buffer.append(i.first);
            buffer.append("\" : ");
            // Print value
            pson_print_value(*i.second, buffer);

            buffer.push_back('\n');
            if (n++ != obj.size() - 1)
                buffer.push_back(',');
        }
        buffer.push_back('}');
    }
}

namespace pson {

    inline std::string PrintValue(Value& v) {
        std::string buffer;
        if (v.type() == pson::JSON_UNKNOW) 
            return buffer;

        imple::pson_print_value(v, buffer);

        return buffer;
    }
}
#endif
