#include "target.h"
#include "piece.h"

namespace Target
{
    Piece::Move _buffer[Board::SIZE];


    bool is_empty()
    {
        for (coord_mch cell = 0; cell < Board::SIZE; cell++)
            if (!Piece::move_is_invalid(get(cell))) return false;

        return true;
    }

    void reset()
    {
        for (coord_mch cell = 0; cell < Board::SIZE; cell++)
            _set(cell, Piece::Move::NONE);
    }
}