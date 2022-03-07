#pragma once
#include "core/utils.h"

namespace Coords
{
    const int_mch _buffer_size = Math::int_length(Board::SIDE) + 1;
    extern char _buffer[_buffer_size];


    // AN - Algebraic Notation   (a1 to h8)
    // EN - Enumarative Notation (0 to 63)

    void _empty_buffer();

    bool convert_AN2EN(coord_mch& coord);
    coord_mch read_and_parse();

    void print_AN(coord_mch coord);
}