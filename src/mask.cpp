#include <Arduino.h>
#include "mask.h"

#include "board.h"

namespace Mask
{
    brd_mch arr[Index::arr_size];
    bool king_is_hurt;
    

    void print(Index mask_index)
    {
        for (coord_mch row = 0; row < Board::SIDE; row++)
        {
            uint8_t value = get(mask_index) >> (row * 8);
            for (coord_mch column = 0; column < Board::SIDE; column++)
            {
                Serial.print(value & 1, BIN);
                Serial.print(F("  "));

                value >>= 1;
            }
            Serial.println();
        }
    }
}