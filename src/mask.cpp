#include "mask.h"
#include "board.h"

namespace Mask
{
    mask_mch _buffer[Index::_buffer_size];
    bool _king_is_hurt;
    

    void print(Index mask_index)
    {
        mserial_plns("   a b c d e f g h");
        mserial_plns(" +-----------------+");
        for (coord_mch row = 0; row < Board::SIDE; row++)
        {
            mserial_p(Board::SIDE-row);
            mserial_ps("| ");

            uint8_t value = get(mask_index) >> (row * Board::SIDE);
            for (coord_mch column = 0; column < Board::SIDE; column++)
            {
                mserial_p(value & 1, BIN);
                mserial_p(' ');

                value >>= 1;
            }
            mserial_pln('|');
        }
        mserial_pln(" +-----------------+");
    }
}