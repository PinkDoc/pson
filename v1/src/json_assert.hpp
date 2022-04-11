#pragma once

#include <stdarg.h>
#include <assert.h>
#include <stdio.h>


namespace pson {

#define PSON_ASSERT(expr,...) \
    do {                        \
        if (!expr) {            \
            fprintf(stderr, "[PSON ASSERT] [FILE : %s] [LINE : %d] [EXPECTION : %s]\n", __FILE__, __LINE__, #__VA_ARGS__);\
            assert(false);       \
        }\
    } while (0)


}