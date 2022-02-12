#pragma once
#include <stdint.h>

#define sizeof_array_mch(x)  sizeof((x))/sizeof((x)[0])

using int_mch   = int8_t;    // general use signed 8-bit integer
using uint_mch  = uint8_t;   // general use unsigned 8-bit integer

using pce_mch   = uint_mch;  // piece describing type (like individual pieces themselves or their props)
using mask_mch  = uint64_t;  // whole board describing type (like i/o masks)
using coord_mch = int_mch;   // board coordinate values

const uint_mch DFLT_MASK_MCH = INT8_MAX + 1;


#if defined(DEBUG)
    #if defined(NDEBUG)
        #error DEBUG and NDEBUG defined simultaneously!
    #endif
#elif !defined(NDEBUG)
    #define NDEBUG
#endif