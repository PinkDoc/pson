#pragma  once

#include "json_value.hpp"

namespace  pson {
namespace imple {
    void pson_print_value(pson::Value& v, std::string& to_print);
    void pson_print_object(pson::Value& v, std::string& to_print);
    void pson_print_array(pson::Value& v, std::string& to_print);

    inline void pson_print_array(pson::Value& v, std::string& to_print)
    {
        using namespace pson;
        Array& a = v.as<Array>();

        if (v.size() == 0)
        {
            to_print.append("[]");
            return;
        }

        to_print.append("[");

        for (auto i = 0; i < a.size(); ++i) {
            pson_print_value(a.at(i), to_print);
            if (i != a.size() - 1)
                to_print.append(" , ");
        }

        to_print.append("]");
    }

    inline void pson_print_object(pson::Value& v, std::string& to_print)
    {
        using namespace pson;
        Object& o = v.as<Object>();

        if (o.size() == 0)
        {
            to_print.append("{}");
        }

        auto& c = o.value_map();

        to_print.append("{\n");

        std::size_t s = 0;

        for (auto& i : c)
        {
            to_print.push_back('"');
            to_print.append(i.first);
            to_print.append("\" : ");

            pson_print_value(i.second, to_print);

            if (s++ != c.size() - 1) {
                to_print.append(", \n");
            }
        }

        to_print.append("\n}");
    }


   inline void pson_print_value(pson::Value& v, std::string& to_print)
   {
       using namespace pson;

       switch (v.type())
       {
           case JSON_NULL:
               to_print.append("null");
               break;
           case JSON_BOOL: {
               const char* b = v.as<Bool>() ? "true" : "false";
               to_print.append(b);
           }
                break;
           case JSON_NUMBER:
               to_print.append(std::to_string(v.as<Number>()));
               break;
           case JSON_STRING:
               to_print.push_back('"');
               to_print.append(v.as<String>());
               to_print.push_back('"');
               break;
           case JSON_ARRAY:
               pson_print_array(v, to_print);
               break;
           case JSON_OBJECT:
               pson_print_object(v, to_print);
               break;
           default:
               return;
       }
   }
}
}

namespace pson {

    inline std::string Value::print()
    {
        std::string ret;
        imple::pson_print_object(*this, ret);
        return std::move(ret);
    }

}
