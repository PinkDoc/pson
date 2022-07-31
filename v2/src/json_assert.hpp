#pragma once

#include <assert.h>

#include <string>
#include <exception>

namespace pson {

#define PSON_ASSERT(expr)   \
    do { \
        if (!expr) {\
            fprintf(stderr, "[PSON-ASSERT][FILE: %s][LINE: %d]\n", __FILE__, __LINE__); \
            assert(true);\
        }\
    }while(0)

}