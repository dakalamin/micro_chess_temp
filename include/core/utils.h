#pragma once
#include <stdint.h>


#if defined(DEBUG)
    #if defined(NDEBUG)
        #error DEBUG and NDEBUG defined simultaneously!
    #endif
#elif !defined(NDEBUG)
    #define NDEBUG
#endif


#define SIZEOF_ARRAY_MCH(x)  sizeof((x))/sizeof((x)[0])

using int_mch   = int8_t;    // general use signed 8-bit integer
using uint_mch  = uint8_t;   // general use unsigned 8-bit integer

using pce_mch   = uint_mch;  // piece describing type (like individual pieces themselves or their props)
using mask_mch  = uint64_t;  // whole board describing type (like i/o masks)
using coord_mch = int_mch;   // board coordinate values
using count_mch = uint16_t;  // integer counters type


const uint_mch DFLT_MASK_MCH = INT8_MAX + 1;
namespace Board
{
    const int_mch SIDE = 8;
    const int_mch SIZE = SIDE*SIDE;
}

namespace ASCII
{
    enum class Bits : char
    {
        NOCASE_CHAR  = 0b00011111,
        LOWERCASE    = 0b00100000,

        LEFTMOST_BIT = 0b01000000
    };

    inline const char to_lower(const char letter)
        { return letter | (char)Bits::LOWERCASE; }
    inline const char to_upper(const char letter)
        { return letter & (~(char)Bits::LOWERCASE); }

    inline constexpr auto encode(const auto value)
        { return (uint_mch)value & (uint_mch)Bits::NOCASE_CHAR; }
    inline const char decode(const auto value)
        { return (char)encode(value) | (char)Bits::LEFTMOST_BIT; }
}