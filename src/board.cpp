#include "board.h"

#include "piece.h"
#include "coords.h"

namespace Board
{
    pce_mch _buffer[Index::_buffer_size][SIZE];
    bool king_is_hurt;


    void fill(Index board_index, const char contents[])
    {
        assert_val_mch(SIZEOF_ARRAY_MCH(contents) != SIZE+1, contents);

        for (coord_mch cell = 0; cell < SIZE; cell++)
        {
            char letter = contents[cell];
            set(board_index, cell, Piece::make(letter, letter == BRD_DEFAULT[cell]));
        }       
    }

    void reset(Index board_index)
    {
        switch (board_index)
        {
            case MAJOR:
                return fill(MAJOR, BRD_DEFAULT);

            case MINOR:
                for (coord_mch cell = 0; cell < SIZE; cell++)
                    set(MINOR, cell, get(MAJOR, cell));

                king_is_hurt = false;
                return;
        }
    }

    void empty(Index board_index, coord_mch cell)
    {
        set(board_index, cell, (pce_mch)Piece::Type::EMPTY);
    }
    
    
    void print_pieces(Index board_index, bool show_shift)
    {
        #ifdef SERIAL_SPEED
            Coords::print_foreach(
            [board_index, show_shift](coord_mch cell)
            {
                pce_mch piece = get(board_index, cell);
                mserial_p(Piece::get_char(piece));

                if (!show_shift) return;
                mserial_p(Piece::is_empty(piece) ? ' ' : (Piece::is_shifted(piece) ? '+' : '-'));
            },
            (show_shift) ? 2 : 1, true );
        #endif
    }

    void print_cellnames()
    {
        #ifdef SERIAL_SPEED
            Coords::print_foreach(
            [](coord_mch cell) { Coords::print_AN(cell); },
            Coords::RANK_LENGTH + 1, false );
        #endif
    }
}