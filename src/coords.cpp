#include "coords.h"
#include "board.h"


namespace Coords
{
    char _buffer[_buffer_size] {'\0'};


    void _empty_buffer()
    {
        for (int_mch i = 0; i < _buffer_size; i++)
            _buffer[i] = '\0';
    }


    bool convert_AN2EN(coord_mch& coord)
    {
        int_mch file = ASCII::to_lower(_buffer[0]) - 'a';
        if (file < 0 || file >= Board::WIDTH) return false;

        coord = 0;
        for (int_mch i = 1; i < _buffer_size; i++)
        {
            int_mch rank = _buffer[i] - '0';

            if (rank < 0 || rank > 9)
                break;
            else
                coord = coord*10 + rank;
        }
        if (coord < 1 || coord > Board::HEIGHT) return false;
        coord = (Board::HEIGHT-coord) * Board::WIDTH + file;

        assert_val_mch(!Board::is_within(coord), coord);
        return true;
    }


#ifdef SERIAL_SPEED
    coord_mch read_and_parse()
    {
        coord_mch coord;

        while (true)
        {
            mserial_ps("AWAITING INPUT >> ");
            while (mserial_find('\n')) { }
            while (!mserial_available())
                delay(50);

            _empty_buffer();
            mserial_read_bytes(_buffer, _buffer_size);

            if (convert_AN2EN(coord)) return coord;
            mserial_ps("INVALID COORDS, PLEASE TRY AGAIN. ");
        }
    }

    void print_AN(coord_mch coord, Axis choice)
    {
        assert_val_mch(!Board::is_within(coord), coord);

        if (match_output(choice, Axis::FILE))
            mserial_p((char)('a' + coord%Board::WIDTH));

        if (match_output(choice, Axis::RANK))
        {   
            int_mch rank = Board::HEIGHT - coord/Board::WIDTH;
            for (int_mch i  = 1; i < Math::int_length(rank); i++)
                mserial_p('0');

            mserial_p(rank, DEC);
        }
    }
    
#else
    coord_mch read_and_parse() { return -1; }
    void print_AN(coord_mch coord, Axis choice) { }
#endif
}