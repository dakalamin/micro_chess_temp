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
    

    inline bool is_within(coord_mch cell_from, coord_mch cell_to)
    {
        return cell_to >= 0 && cell_to < SIZE &&
               abs((cell_from % SIDE) - (cell_to % SIDE)) < 3;
    }


    inline pce_mch* get(Index board_index)
        { return arr[board_index]; }

    void fill(Index board_index, const char contents[]);
    void reset(Index board_index);
    void empty(Index board_index, coord_mch cell);


    template <typename Func>
    void print(Func function);

    void print_pieces(Index board_index);
    void print_cellnames();
}