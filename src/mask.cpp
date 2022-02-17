#include "core/micro_serial.h"
#include "mask.h"

#include "board.h"

namespace Mask
{
    mask_mch _arr[Index::_arr_size];
    bool _king_is_hurt;
    

    void print(Index mask_index)
    {
        for (coord_mch row = 0; row < Board::SIDE; row++)
        {
            uint8_t value = get(mask_index) >> (row * Board::SIDE);
            for (coord_mch column = 0; column < Board::SIDE; column++)
            {
                mserial_p(value & 1, BIN);
                mserial_ps("  ");

                value >>= 1;
            }
            mserial_pln();
        }
        mserial_pln();
    }
}