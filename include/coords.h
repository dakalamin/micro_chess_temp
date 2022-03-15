#pragma once
#include "core/utils.h"

// AN - Algebraic Notation   (a1 to h8)
// EN - Enumarative Notation (0 to 63)
namespace Coords
{
    const int_mch RANK_LENGTH = Math::int_length(Board::HEIGHT);

    const int_mch _buffer_size = RANK_LENGTH + 1;
    extern char _buffer[_buffer_size];

    enum Axis : int_mch
    {
        FILE = 0b01,
        RANK = 0b10,
        BOTH = FILE|RANK
    };
    inline bool match_output(Axis choice, Axis compare)
        { return choice & compare; }


    void _empty_buffer();

    bool convert_AN2EN(coord_mch& coord);
    coord_mch read_and_parse();

    void print_AN(coord_mch coord, Axis choice=Axis::BOTH);


#ifdef SERIAL_SPEED
    inline void _print_line(int_mch output_width)
    {
        for (int_mch i = 0; i < RANK_LENGTH + 1; i++) mserial_p(' ');
        mserial_p('+');
        for (int_mch i = 0; i < 1 + Board::WIDTH*(output_width+1); i++) mserial_p('-');
        mserial_pln('+');
    }
    
    inline void print_foreach(auto function, int_mch output_width, bool print_axes=true)
    {
        if (print_axes)
        {
            for (int_mch i = 0; i < RANK_LENGTH + 3; i++) mserial_p(' ');
            for (int_mch i = 0; i < Board::WIDTH; i++)
            {
                print_AN(i, Axis::FILE);
                for (int_mch j = 0; j < output_width; j++)
                    mserial_p(' ');
            }
            mserial_pln();
            _print_line(output_width);
        }

        for (coord_mch row = 0; row < Board::SIZE; row += Board::WIDTH)
        {
            if (print_axes)
            {
                print_AN(row, Axis::RANK);
                mserial_ps(" | ");
            }

            for (coord_mch column = 0; column < Board::WIDTH; column++)
            {
                function(row + column);
                mserial_p(' ');
            }
            mserial_pln((print_axes) ? '|' : ' ');
        }

        if (print_axes) _print_line(output_width);
        mserial_pln();
    }

#else
    inline void _print_line(int_mch output_width) { }
    inline void print_foreach(auto function, int_mch output_width, bool print_axes=true) { }
#endif
}