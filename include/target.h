#pragma once
#include "core/utils.h"

#include "move.h"
#include "board.h"
#include "coords.h"

namespace Target
{
    extern Piece::Move _buffer[Board::SIZE];


    inline Piece::Move& get(coord_mch cell)
        { return _buffer[cell]; }
    inline void _set(coord_mch cell, Piece::Move move)
        { get(cell) = move; }

    bool is_empty();
    void reset();

#ifdef SERIAL_SPEED
    inline void print()
    {
        Coords::print_foreach(
        [](coord_mch cell) { mserial_p(Piece::convert_move2char(get(cell), true)); },
        1, true);
    }
#else
    inline void print() { }
#endif
}