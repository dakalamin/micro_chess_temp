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
        { return choice | compare; }


    void _empty_buffer();

    bool convert_AN2EN(coord_mch& coord);
    coord_mch read_and_parse();

    void print_AN(coord_mch coord, Axis choice=Axis::BOTH);

    void print_foreach(auto function, int_mch output_width, bool print_axes=true);
    void _print_line(int_mch output_width);
}