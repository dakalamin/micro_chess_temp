#pragma once

#if defined(DEBUG)
    #if defined(NDEBUG)
        #error DEBUG and NDEBUG defined simultaneously!
    #endif
#elif !defined(NDEBUG)
    #define NDEBUG
#endif

#include <stdint.h>
#include <Arduino.h>
#include "core/micro_serial.h"
#include "core/micro_assert.h"

#define SIZEOF_ARRAY_MCH(x)  (sizeof((x))/sizeof((x)[0]))

using int_mch   = int8_t;    // general use signed 8-bit integer
using uint_mch  = uint8_t;   // general use unsigned 8-bit integer

using pce_mch   = uint_mch;  // piece describing type (like individual pieces themselves or their props)
using mask_mch  = uint64_t;  // whole board describing type (like i/o masks)
using coord_mch = int_mch;   // board coordinate values
using count_mch = uint16_t;  // integer counters type


const uint_mch LEFTMOST_BIT = INT8_MAX + 1;
namespace Board
{
    #define _MIN_SIDE 6
    #define _MAX_SIDE 26  // number of letters in english alphabet

    const int_mch MIN_SIDE = _MIN_SIDE;
    const int_mch MAX_SIDE = _MAX_SIDE;

    #ifdef BOARD_SIDE
        #if   BOARD_SIDE < _MIN_SIDE
            #error Board is too small! Minimal size is 6x6
        #elif BOARD_SIDE > _MAX_SIDE
            #error Board is too big! Maximum size is 26x26
        #endif
    #else
        #define BOARD_SIDE 8
    #endif

    const int_mch SIDE = BOARD_SIDE;
    const int_mch SIZE = SIDE*SIDE;

    //#undef _MIN_SIDE
    //#undef _MAX_SIDE
    //#undef BOARD_SIDE
}

namespace ASCII
{
    const char NULLCHAR = '\0';
    enum Bits : char
    {
        NOCASE_CHAR = 0b00011111,  // unique ASCII letter bits
        LOWERCASE   = 0b00100000,  // bit indicating if letter is lowercase
        MSCB        = 0b01000000   // Most Significant Char Bit
    };

    inline constexpr char to_lower(const char letter)
        { return letter | LOWERCASE; }
    inline constexpr char to_upper(const char letter)
        { return letter & (~LOWERCASE); }

    inline constexpr auto encode(const auto value)
        { return (uint_mch)value & NOCASE_CHAR; }
    inline constexpr char decode(const auto value)
        { return encode(value) | MSCB; }
}

namespace Math
{
    inline constexpr int_mch int_length(auto integer)
    {
        int_mch length = 0;
        do {
            integer /= 10; length++;
        } while(integer != 0);

        return length;
    }
}