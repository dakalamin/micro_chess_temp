#include "coords.h"
#include "board.h"

namespace Coords
{
    char _buffer[_buffer_size] {ASCII::NULLCHAR};


    void _empty_buffer()
    {
        for (int_mch i = 0; i < _buffer_size; i++)
            _buffer[i] = ASCII::NULLCHAR;
    }


    bool convert_AN2EN(coord_mch& coord)
    {
        int_mch file = ASCII::to_lower(_buffer[0]) - 'a';
        if (file < 0 || file >= Board::SIDE) return false;

        coord = 0;
        for (int_mch i = 1; i < _buffer_size; i++)
        {
            int_mch rank = _buffer[i] - '0';

            if (rank < 0 || rank > 9)
                break;
            else
                coord = coord*10 + rank;
        }
        if (coord < 1 || coord > Board::SIDE) return false;
        coord = (Board::SIDE-coord) * Board::SIDE + file;

        assert_val_mch(!Board::is_within(coord), coord);
        return true;
    }

    coord_mch read_and_parse()
    {
        coord_mch coord;

        while (true)
        {
            mserial_ps("AWAITING INPUT >> ");
            while (mserial_find('\n')) { }
            while (!Serial.available())
                delay(50);

            _empty_buffer();
            mserial_read_bytes(_buffer, _buffer_size);

            if (convert_AN2EN(coord)) return coord;
            mserial_ps("INVALID COORDS, PLEASE TRY AGAIN. ");
        }
    }


    void print_AN(coord_mch coord)
    {
        assert_val_mch(!Board::is_within(coord), coord);

        mserial_p((char)('A' + coord%Board::SIDE));
        mserial_p(Board::SIDE - coord/Board::SIDE, DEC);
    }
}