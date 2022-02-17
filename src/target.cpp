#include "core/micro_serial.h"
#include "target.h"
#include "piece.h"

namespace Target
{
    Piece::Move _arr[Board::SIZE];


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
        for (coord_mch row = 0; row < Board::SIDE; row++)
        {
            for (coord_mch column = 0; column < Board::SIDE; column++)
            {
                mserial_p(Piece::move2char(get(row*Board::SIDE + column)));
                mserial_ps("  ");
            }
            mserial_pln(' ');
        }
    }
}