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

template <typename T> 
struct minmax { T MIN; T MAX; };


namespace Board
{
    #define _MIN_WIDTH  6
    #define _MIN_HEIGHT 6
    #define _MAX_WIDTH  26  // number of letters in english alphabet
    #define _MAX_HEIGHT 99

    const int_mch MIN_WIDTH  = _MIN_WIDTH;
    const int_mch MIN_HEIGHT = _MIN_HEIGHT;
    const int_mch MAX_WIDTH  = _MAX_WIDTH;
    const int_mch MAX_HEIGHT = _MAX_HEIGHT;

    #ifdef BOARD_SIDE
        #if defined(BOARD_WIDTH) || defined(BOARD_HEIGHT)
            #error Eiter BOARD_SIDE or both BOARD_WIDTH & BOARD_HEIGHT can be defined, but not simulataniously
        #elif BOARD_SIDE < min(_MIN_WIDTH, _MIN_HEIGHT)
            #error Board is too small! Minimal size is 6x6
        #elif BOARD_SIDE > min(_MAX_WIDTH, _MAX_HEIGHT)
            #error Board is too big! Maximum size is 26x26
        #endif

        #define BOARD_WIDTH  BOARD_SIDE
        #define BOARD_HEIGHT BOARD_SIDE
    #elif defined(BOARD_WIDTH) && defined(BOARD_HEIGHT)
        #if   BOARD_WIDTH  < MIN_WIDTH
            #error BOARD_WIDTH is too small! Must be in interval between 6 and 26
        #elif BOARD_WIDTH  > MAX_WIDTH
            #error BOARD_WIDTH is too big! Must be in interval between 6 and 26
        #elif BOARD_HEIGHT < MIN_HEIGHT
            #error BOARD_HEIGHT is too small! Must be in interval between 6 and 99
        #elif BOARD_HEIGHT > MAX_HEIGHT
            #error BOARD_HEIGHT is too big! Must be in interval between 6 and 99
        #endif
    #else
        #error Eiter BOARD_SIDE or both BOARD_WIDTH & BOARD_HEIGHT must be defined
    #endif

    const int_mch WIDTH  = BOARD_WIDTH;
    const int_mch HEIGHT = BOARD_HEIGHT;
    const int_mch SIZE   = WIDTH*HEIGHT;

    #undef _MIN_WIDTH
    #undef _MIN_HEIGHT
    #undef _MAX_WIDTH
    #undef _MAX_HEIGHT
    
    #undef BOARD_WIDTH
    #undef BOARD_HEIGHT

    #ifdef BOARD_SIDE
        #undef BOARD_SIDE
    #endif
}

const uint_mch LEFTMOST_BIT = INT8_MAX + 1;
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