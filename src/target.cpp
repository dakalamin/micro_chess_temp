#include "target.h"
#include "piece.h"
#include "coords.h"

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

    void print()
    {
        Coords::print_foreach(
        [](coord_mch cell) { mserial_p(Piece::convert_move2char(get(cell), true)); },
        1, true);
    }
}