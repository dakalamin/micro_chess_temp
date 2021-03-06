#pragma once
#include "core/utils.h"

namespace Mask
{
    enum Index : uint_mch
    {
        IN,
        FRONTLINE,
        REGICIDES,
        MENACES,

        _buffer_size
    };
    extern mask_mch _buffer[_buffer_size];
    extern bool _king_is_hurt;


    inline mask_mch& get(Index mask_index)
        { return _buffer[mask_index]; }
    inline bool check(coord_mch cell, Index mask_index)
        { return (get(mask_index) >> cell) & 1; };

    inline void set(coord_mch cell, Index mask_index)
        { get(mask_index) |= ((mask_mch)1 << cell); };
    inline void reset(Index mask_index)
        { get(mask_index) = 0; }

    void print(Index mask_index);
}