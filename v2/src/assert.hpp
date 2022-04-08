#pragma once

#include <assert.h>

#include <string>
#include <exception>

namespace pson {
    /*
    class bad_conversion : std::exception {
    private:
        std::string msg_;
    public:

        bad_conversion():
            msg_("Bad conversion!")
        {}

        std::string what() {
            return msg_;
        }
    };
    */

#define PSON_ASSERT(expr)   \
    do { \
        if (!expr) {\
            fprintf(stderr, "[PSON-ASSERT][FILE: %s][LINE: %d]\n", __FILE__, __LINE__); \
            assert(true);\
        }\
    }while(0)

}
