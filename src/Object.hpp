//
// Created by pink on 2022/2/26.
//

#ifndef PNET_PSON_OBJECT_HPP
#define PNET_PSON_OBJECT_HPP

#include "Value.hpp"

#include <map>
#include <string>

namespace pson {

    class Value;

    class Object {
    private:

        std::map<std::string, Value*> value_map_;

    public:
        explicit
        Object() {}

        int parse(std::string& json_data);
        int parse(char* json_data, size_t len);

        bool Has(const std::string& name) { return value_map_.find(name) != value_map_.end(); }
        Value& Get(const std::string& name) { return *value_map_[name]; }
        size_t Size() const { return value_map_.size(); }

    };
}   // namespace pson
#endif
