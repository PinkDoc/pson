#ifndef PNET_PSON_ARRAY_HPP
#define PNET_PSON_ARRAY_HPP

#include <vector>

#include "Value.hpp"

namespace pson {

    class Value;

    class Array {
    private:
        std::vector<Value*> values_;
    public:
        explicit
        Array() noexcept {}

        bool     Has(unsigned int i) { return values_.size() > i; }
        Value&   Get(unsigned int i) { return *values_[i]; }
        void     Push(Value* val) { values_.emplace_back(val); }
        size_t   Size() const { return values_.size(); }

    };

}   // namespace pson

#endif