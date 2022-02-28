#ifndef PNET_PSON_ARRAY_HPP
#define PNET_PSON_ARRAY_HPP

#include <vector>

#include "Value.hpp"

namespace pson {

    class Value;

    class Array {
    private:
        std::vector<Value> values_;
    public:
        explicit
        Array() noexcept {}

        Array(const Array& array):values_(array.values_) {}

        Array(Array&& array):values_(std::move(array.values_))
        {}

        void operator= (Array&& array)
        {
            values_ = std::move(array.values_);
        }

        void operator= (Array& array)
        {
            values_ = array.values_;
        }

        bool     Has(size_t i) { return values_.size() > i; }
        Value&   Get(size_t i) { return values_[i]; }
        void     Push(Value val) { values_.emplace_back(std::move(val)); }
        size_t   Size() const { return values_.size(); }
    };



}   // namespace pson

#endif