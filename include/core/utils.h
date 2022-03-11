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


namespace Board
{
    #define _MIN_SIDE 6
    #define _MAX_SIDE 26  // number of letters in english alphabet

    #ifdef BOARD_SIDE
        #if defined(BOARD_WIDTH) || defined(BOARD_HEIGHT)
            #error Eiter BOARD_SIDE or both BOARD_WIDTH & BOARD_HEIGHT can be defined, but not simulataniously
        #elif BOARD_SIDE < _MIN_SIDE
            #error BOARD_SIDE is too small! Must be in interval between 6 and 26
        #elif BOARD_SIDE > _MAX_SIDE
            #error BOARD_SIDE is too big! Must be in interval between 6 and 26
        #endif

        #define BOARD_WIDTH  BOARD_SIDE
        #define BOARD_HEIGHT BOARD_SIDE
        
    #elif defined(BOARD_WIDTH) && defined(BOARD_HEIGHT)
        #if   BOARD_WIDTH  < _MIN_SIDE
            #error BOARD_WIDTH is too small!  Must be in interval between 6 and 26
        #elif BOARD_WIDTH  > _MAX_SIDE
            #error BOARD_WIDTH is too big!    Must be in interval between 6 and 26
        #elif BOARD_HEIGHT < _MIN_SIDE
            #error BOARD_HEIGHT is too small! Must be in interval between 6 and 26
        #elif BOARD_HEIGHT > _MAX_SIDE
            #error BOARD_HEIGHT is too big!   Must be in interval between 6 and 26
        #endif
    #else
        #error Either BOARD_SIDE or both BOARD_WIDTH & BOARD_HEIGHT must be defined!
    #endif

    const int_mch DEFAULT_SIDE = 8;

    const int_mch WIDTH  = BOARD_WIDTH;
    const int_mch HEIGHT = BOARD_HEIGHT;
    const coord_mch SIZE = WIDTH*HEIGHT;

    #undef _MIN_SIDE
    #undef _MAX_SIDE
    
    #undef BOARD_WIDTH
    #undef BOARD_HEIGHT

    #ifdef BOARD_SIDE
        #undef BOARD_SIDE
    #endif
}

const uint_mch LEFTMOST_BIT = INT8_MAX + 1;
namespace ASCII
{
    enum Bits : char
    {
        NOCASE    = 0b00011111,  // unique ASCII letter bits
        LOWERCASE = 0b00100000,  // bit indicating if letter is lowercase
        MSCB      = 0b01000000   // Most Significant Char Bit
    };

    inline constexpr char to_lower(const char letter)
        { return letter | LOWERCASE; }
    inline constexpr char to_upper(const char letter)
        { return letter & (~LOWERCASE); }

    inline constexpr auto encode(const auto value)
        { return (uint_mch)value & NOCASE; }
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
        } while (integer != 0);

        return length;
    }
}