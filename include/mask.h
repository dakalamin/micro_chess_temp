#pragma once
#include "core/utils.h"
#include "coords.h"

namespace Mask
{
    inline constexpr coord_mch _calc(coord_mch cell) { return (1 + cell) / 8; }

    enum Index : uint_mch
    {
        IN,
        FRONTLINE,
        REGICIDES,
        MENACES,

        _buffer_index_size
    };
    const  int_mch _buffer_segment_size = _calc(Board::SIZE) + 1;
    extern int_mch _buffer[_buffer_index_size][_buffer_segment_size];


    inline int_mch* get(Index mask_index)                 { return _buffer[mask_index]; }
    inline int_mch& get(coord_mch cell, Index mask_index) { return get(mask_index)[_calc(cell)]; }
    inline bool check(coord_mch cell, Index mask_index)   { return (get(cell, mask_index) >> (cell % 8)) & 1; };
    bool check(Index mask_index);

    inline void set(coord_mch cell, Index mask_index)     { get(cell, mask_index) |= (1 << (cell % 8)); }
    void reset(Index mask_index);


    inline void print(Index mask_index)
    {
        #ifdef SERIAL_SPEED
            Coords::print_foreach(
            [mask_index](coord_mch cell) { mserial_p(check(cell, mask_index), BIN); },
            1, true);
        #endif
    }
}