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

        std::map<std::string, Value> value_map_;

    public:
        explicit
        Object() noexcept {}

        Object(const Object& obj): value_map_(obj.value_map_)
        {}

        Object(Object&& obj): value_map_(std::move(obj.value_map_))
        {}

        void operator= (Object&& obj)
        {
            value_map_ = std::move(obj.value_map_);
        }

        void operator= (Object& obj)
        {
            value_map_ = obj.value_map_;
        }

        void Insert(std::string name, Value val) { value_map_.emplace(std::move(name) , std::move(val)); }
        void Insert(std::string name, Value val) { value_map_.emplace(std::move(name), std::move(val)); }
        bool Has(const std::string& name) { return value_map_.find(name) != value_map_.end(); }
        Value& Get(const std::string& name) { return value_map_[name]; }
        size_t Size() const { return value_map_.size(); }

    };
}   // namespace pson
#endif
