#pragma once
#include <Arduino.h>
#include "core/utils.h"
#include "board_patterns.h"

namespace Board
{
    const int_mch SIDE = 8;
    const int_mch SIZE = SIDE*SIDE;

    enum Index : uint_mch
    {
        MAJOR,
        MINOR,

        arr_size
    };
    extern pce_mch arr[Index::arr_size][SIZE];
    
    inline bool is_within(coord_mch cell)
        { return (cell >= 0) && (cell < SIZE); }
    inline bool is_within(coord_mch cell_from, coord_mch cell_to)
        { return is_within(cell_from) && is_within(cell_to) && (abs((cell_from % SIDE) - (cell_to % SIDE)) < 3); }

    inline pce_mch* _get(Index board_index)
        { return arr[board_index]; }
    inline pce_mch& get(Index board_index, coord_mch cell)
        { return _get(board_index)[cell]; }

    void fill(Index board_index, const char contents[]);
    void reset(Index board_index);

    void empty(Index board_index, coord_mch cell);
    void set(Index board_index, coord_mch cell, pce_mch piece)
        { get(board_index, cell) = piece; }


    template <typename Func>
    void _print(Func function);

    void print_pieces(Index board_index);
    void print_cellnames();
}