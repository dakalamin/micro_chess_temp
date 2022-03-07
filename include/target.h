#pragma once
#include "core/utils.h"

#include "move.h"
#include "board.h"

namespace Target
{
    extern Piece::Move _buffer[Board::SIZE];


    inline Piece::Move& get(coord_mch cell)
        { return _buffer[cell]; }
    inline void _set(coord_mch cell, Piece::Move move)
        { get(cell) = move; }

    bool is_empty();
    void reset();
    void print();
}