#pragma once

#include <math.h>

#include "json_value.hpp"

namespace pson {

    class Parser {
    private:

        struct state {
            char* data_;
            size_t size_;
            size_t offset_;

            inline state(char* data, size_t size):
                    data_(data),
                    size_(size),
                    offset_(0) {}

            inline state():
                    data_(nullptr),
                    size_(0),
                    offset_(0) {}

            inline void reset() { data_ = nullptr; size_ = 0; offset_ = 0; }
            inline void set(char* data, size_t len) { reset(); data_ = data; size_ = len; }
        };

        state state_;

        // Insignificant whitespace is allowed before or after any of the six
        //   structural characters.
        //
        //      ws = *(
        //                %x20 /              ; Space
        //                %x09 /              ; Horizontal tab
        //                %x0A /              ; Line feed or New line
        //                %x0D                ; Carriage return
        //            )
        void skip_write_blank();

        // [name] : true
        bool parse_true(Value& v);

        // [name] : false
        bool parse_false(Value& v);

        // [name] : null
        bool parse_null(Value& v);

        // :)
        bool parse_number(Value& v);

        //  0xffff
        bool parse_hex4(unsigned& u);

        bool parse_utf8(String & v, unsigned u);

        // Only parse string
        bool parse_string_row(String& s);

        // parse value string
        bool parse_string(Value& v);

        // [name]: [val1, val2, val3......]
        bool parse_array(Value& v);

        bool parse_object(Value& v);

        bool parse_value(Value& v);
    public:

        inline Parser(const std::string& data):
                state_(const_cast<char*>(data.data()) , data.size()) {}

        inline Parser(char* data, size_t size):
                state_(const_cast<char*>(data ), size) {}

        bool Parse(Value& v);

        inline void Reset() { state_.reset(); }
        inline void SetData(char* data, size_t len) { Reset(); state_.set(data, len); }
        inline void SetData(std::string& data) { Reset(); state_.set(const_cast<char*>(data.data()), data.size()); }

    };

    inline void Parser::skip_write_blank()
    {
        auto& offset = state_.offset_;
        auto d = state_.data_;
        while(offset < state_.size_)
        {
            auto ch = d[offset];
            if (ch == 0x20 || ch == 0x09 || ch == 0x0A || ch == 0x0D ) offset++;
            else break;
        }
    }

    inline bool Parser::parse_true(Value &v)
    {
        auto& offset = state_.offset_;
        auto d = state_.data_;
        PSON_ASSERT(d[offset] == 't');

        if (offset + 3 >= state_.size_ ||
            d[offset + 1] != 'r' ||
            d[offset + 2] != 'u' ||
            d[offset + 3] != 'e') return false;

        offset += 4;
        v.reset_as<Bool>();
        v.as<Bool>() = true;

        return true;
    }

    inline bool Parser::parse_false(Value &v)
    {
        auto& offset = state_.offset_;
        auto d = state_.data_;
        PSON_ASSERT(d[offset] == 'f');

        if (offset + 4 >= state_.size_ ||
            d[offset + 1] != 'a' ||
            d[offset + 2] != 'l' ||
            d[offset + 3] != 's' ||
            d[offset + 4] != 'e') return false;

        offset += 5;
        v.reset_as<Bool>();
        v.as<Bool>() = false;

        return true;
    }

    inline bool Parser::parse_null(Value &v)
    {
        auto& offset = state_.offset_;
        auto d = state_.data_;
        PSON_ASSERT(d[offset] == 'n');

        if (offset + 3 >= state_.size_ ||
            d[offset + 1] != 'u' ||
            d[offset + 2] != 'l' ||
            d[offset + 3] != 'l') return false;

        offset += 4;
        v.reset_as<Null>();

        return true;
    }

    inline bool Parser::parse_number(Value &v)
    {
#define ISDIGIT(ch) (ch >= '0' && ch <= '9')
#define ISONETONINE(ch) (ch >= '1' && ch <= '9')

        const char* begin = state_.data_ + state_.offset_;
        char* end = const_cast<char*>(begin);

        if (*end == '-') end++;
        if (*end == '0') end++;
        else {
            if (!ISONETONINE(*end)) return false;   // 00.1
            for(;ISDIGIT(*end); end++);
        }
        if (*end == '.')
        {
            end++;
            if (!ISDIGIT(*end)) return false;       // 0.e1
            for(;ISDIGIT(*end); end++);
        }
        if (*end == 'e' || *end == 'E')
        {
            end++;
            if (*end == '+' || *end == '-') end++;
            if (!ISDIGIT(*end)) return false;
            for(;ISDIGIT(*end); end++);
        }
        errno = 0;
        Number res = strtod(begin , nullptr);
        if (errno == ERANGE || res == HUGE_VAL || res == -HUGE_VAL) return false;       // Number too big !
        v.reset_as<Number>();
        v.as<Number>() = res;
        state_.offset_ += end - begin;
        return true;

#undef ISONETONINE
#undef ISDIGIT

    }

    inline bool Parser::parse_hex4(unsigned& u)
    {
        int i;
        u = 0;
        char* d = state_.data_;
        auto& offset = state_.offset_;
        for(i = 0; i < 4; i++)
        {
            char ch = d[offset++];
            u <<= 4;
            if (ch >= '0' && ch <= '9') u |= ch - '0';
            else if (ch >= 'A' && ch <= 'F') u |= ch - ('A' - 10);
            else if (ch >= 'a' && ch <= 'f') u |= ch - ('a' - 10);
            else return false;
        }
        return true;
    }

    inline bool Parser::parse_utf8(String &s, unsigned u)
    {
        if (u <= 0x7F)
        {
            s.push_back(u & 0xFF);
        }
        else if (u <= 0x7FF)
        {
            s.push_back(0xC0 | (u >> 6) & 0xFF);
            s.push_back(0x80 | (u & 0x3F));
        }
        else if (u <= 0xFFFF)
        {
            s.push_back(0xE0 | ((u >> 12) & 0xFF));
            s.push_back(0x80 | ((u >> 6) & 0x3F));
            s.push_back(0x80 | (u & 0x3F));
        }
        else
        {
            PSON_ASSERT(u <= 0x10FFFF);
            s.push_back(0xF0 | ((u >> 18) & 0xFF));
            s.push_back(0x80 | ((u >> 12) & 0x3F));
            s.push_back(0x80 | ((u >> 6) & 0x3F));
            s.push_back(0x80 | (u & 0x3F));
        }
        return true;
    }

    inline bool Parser::parse_string_row(String& s)
    {
        auto& offset = state_.offset_;
        auto d = state_.data_;
        unsigned u{0}, u2{0};
        PSON_ASSERT(d[offset] == '\"');
        ++offset;
        while (true)
        {
            if (offset >= state_.size_) return false;
            char ch = d[offset];
            switch (ch) {
                case '\"': {offset++; return true; }
                case '\0': return false;
                case '\\':
                    switch (d[offset++]) {
                        case '\"': s.push_back('\"'); break;
                        case '\\': s.push_back('\\'); break;
                        case '/': s.push_back('/'); break;
                        case 'b': s.push_back('\b'); break;
                        case 'f': s.push_back('\f'); break;
                        case 'n': s.push_back('\n'); break;
                        case 'r': s.push_back('\r'); break;
                        case 't': s.push_back('\t'); break;
                        case 'u':
                            if (!parse_hex4(u)) return false;
                            if (u >= 0xD800 & u <= 0xDBFF)
                            {
                                if (d[offset++] != '\\') return false;
                                if (d[offset++] != 'u') return false;
                                if (!parse_hex4(u2)) return false;
                                if (u2 < 0xDC00 || u2 > 0xDFFF) return false;
                                u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
                            }
                            parse_utf8(s, u);
                            break;
                    }
                default: {
                    s.push_back(ch);
                    ++offset;
                }
            }
        }

    }

    inline bool Parser::parse_string(Value &v)
    {
        String s;
        if (!parse_string_row(s)) return false;
        v.reset_as<String>();
        v.as<String>() = std::move(s);
        return true;
    }

    inline bool Parser::parse_array(Value &v)
    {
        auto& offset = state_.offset_;
        char* d = state_.data_;
        PSON_ASSERT(d[offset] == '[');
        ++offset;
        skip_write_blank();
        if (offset >= state_.size_) return false;

        Array array;
        v.reset_as<Array>();
        v.as<Array>() = std::move(array);

        if (d[offset] == ']')
        {
            offset++;
            return true;
        }

        while(true)
        {
            Value val;
            if (!Parse(val)) return false;
            v.as<Array>().push_back(val);
            skip_write_blank();

            if (d[offset] == ',')
                offset++;
            else if (d[offset] == ']')
            {
                offset++;
                return true;
            }
            else
                return false;

        }
    }

    inline bool Parser::parse_object(Value &v)
    {
        auto& offset = state_.offset_;
        char* d = state_.data_;
        PSON_ASSERT(d[offset] == '{');
        ++offset;
        skip_write_blank();

        if (offset >= state_.size_) {
            return false;
        }

        Object obj;
        v.reset_as<Object>();
        v.as<Object>() = std::move(obj);

        if (d[offset] == '}')
        {
            offset++;
            return true;
        }

        while(true)
        {
            String name;
            if (!parse_string_row(name)) return false;
            skip_write_blank();

            Value val;
            char ch = d[offset];
            if (d[offset] != ':') return false;
            ++offset;

            auto ret = Parse(val);
            if (!ret) return false;
            v.as<Object>().insert(std::move(name), val);

            skip_write_blank();
            if (d[offset] == ',') {
                ++offset;
                skip_write_blank();
            }
            else if (d[offset] == '}')
            {
                ++offset;
                return true;
            }
            else if (offset >= state_.size_)
            {
                return false;
            }
        }
    }

    inline bool Parser::parse_value(Value& v)
    {
        char ch = state_.data_[state_.offset_];
        switch (ch) {
            case 't':
                return parse_true(v);
            case 'f':
                return parse_false(v);
            case 'n':
                return parse_null(v);
            case '\"':
                return parse_string(v);
            case '[':
                return parse_array(v);
            case '{':
                return parse_object(v);
            default:
                return parse_number(v);
        }
        // Make happy
        return false;
    }

    inline bool Parser::Parse(Value& v)
    {
        skip_write_blank();
        return parse_value(v);
    }

    // Implement for value
    inline bool Value::parse(const std::string &json_data)
    {
        Parser p(json_data);
        return p.Parse(*this);
    }

    inline bool Value::parse(char *json_data, std::size_t len)
    {
        Parser p(json_data, len);
        return p.Parse(*this);
    }

}