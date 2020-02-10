#pragma once

// Surround includes in these defines to silence warnings
// from third party libraries.

#if defined _MSC_VER
/* DISABLE WARNINGS */
#define DISABLE_WARNINGS __pragma(warning( push, 0 ))
/* ENABLE WARNINGS */
    #define ENABLE_WARNINGS __pragma(warning( pop ))
#elif defined __GNUC__
/* DISABLE WARNINGS */
    #define DISABLE_WARNINGS \
        _Pragma("GCC diagnostic push") \
        _Pragma("GCC diagnostic ignored \"-Wall\"")
        _Pragma("GCC diagnostic ignored \"-Wcpp\"")
/* ENABLE WARNINGS */
    #define ENABLE_WARNINGS _Pragma("GCC diagnostic push")
#else  /* TODO: add more compilers here */
    #define DISABLE_WARNINGS
    #define ENABLE_WARNINGS
#endif

#include <exception>
// Override rapidjson assertions to throw exceptions by default
#undef RAPIDJSON_ASSERT
#define RAPIDJSON_ASSERT(x) if(!(x)) throw std::runtime_error("JSON Parsing failed. Check your config!");
