#pragma once
#include "core/utils.h"

#include "move.h"
#include "coords.h"

namespace Target
{
    extern Piece::Move _buffer[Board::SIZE];


    inline Piece::Move& get(coord_mch cell)
        { return _buffer[cell]; }
    inline void set(coord_mch cell, Piece::Move move)
        { get(cell) = move; }

    bool is_empty();
    void reset();


    inline void print()
    {
        #ifdef SERIAL_SPEED
            Coords::print_foreach(
            [](coord_mch cell) { mserial_p(Piece::convert_move2char(get(cell), true)); },
            1, true);
        #endif
    }
}