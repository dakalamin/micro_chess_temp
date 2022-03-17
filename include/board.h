#pragma once
#include "core/utils.h"
#include "board_patterns.h"

namespace Board
{
    enum Index : uint_mch
    {
        MAJOR,
        MINOR,

        _buffer_size
    };
    extern pce_mch _buffer[Index::_buffer_size][SIZE];
    extern bool king_is_hurt;

    
    inline bool is_within(coord_mch cell)
        { return (cell >= 0) && (cell < SIZE); }
    inline bool is_within(coord_mch cell_from, coord_mch cell_to)
        { return is_within(cell_from) && is_within(cell_to) && (abs((cell_from % WIDTH) - (cell_to % WIDTH)) < 3); }

    inline pce_mch* _get_arr(Index board_index)
        { return _buffer[board_index]; }
    inline pce_mch& get(Index board_index, coord_mch cell)
        { return _get_arr(board_index)[cell]; }

    void fill(Index board_index, const char contents[]);
    void reset(Index board_index);

    void empty(Index board_index, coord_mch cell);
    inline void set(Index board_index, coord_mch cell, pce_mch piece)
        { get(board_index, cell) = piece; }

    void print_pieces(Index board_index, bool show_shift=false);
    void print_cellnames();
}