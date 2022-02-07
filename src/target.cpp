#include <Arduino.h>
#include "target.h"

namespace Target
{
    Piece::Move arr[Board::SIZE];


    bool is_empty()
    {
        for (coord_mch cell = 0; cell < Board::SIZE; cell++)
            if (Piece::move_is_valid(get(cell))) return false;

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
                Serial.print((uint_mch)get(row*Board::SIDE + column), DEC);
                Serial.print(F("  "));
            }
            Serial.println();
        }
    }
}