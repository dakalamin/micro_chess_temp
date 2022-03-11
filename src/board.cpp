#include "board.h"

#include "game.h"
#include "mask.h"
#include "piece.h"
#include "target.h"
#include "coords.h"

namespace Board
{
    pce_mch _buffer[Index::_buffer_size][SIZE];


    void fill(Index board_index, const char contents[])
    {
        assert_val_mch(SIZEOF_ARRAY_MCH(contents) != SIZE+1, contents);

        for (coord_mch cell = 0; cell < SIZE; cell++)
        {
            char new_piece = contents[cell];
            assert_val_mch(Piece::_char_is_valid(new_piece), contents);

            pce_mch      piece = (pce_mch)Piece::Type::EMPTY;;
            Piece::Shift shift = Piece::Shift::FALSE;

            if (new_piece != Piece::EMPTY_CHAR)
            {
                piece  = (pce_mch)Piece::_get_type((pce_mch)new_piece);
                piece |= ((pce_mch)Piece::_get_color((pce_mch)new_piece) ^ (pce_mch)Piece::Color::WHITE);

                if (new_piece != BRD_DEFAULT[cell])
                    shift = Piece::Shift::TRUE;
            }

            set(board_index, cell, piece|(pce_mch)shift);
        }
    }

    void reset(Index board_index)
    {
        switch (board_index)
        {
            case MAJOR:
                fill(MAJOR, BRD_DEFAULT);
                return;

            case MINOR:
                for (coord_mch cell = 0; cell < SIZE; cell++)
                    set(MINOR, cell, get(MAJOR, cell));

                Mask::_king_is_hurt = false;
                return;
        }
    }

    void empty(Index board_index, coord_mch cell)
    {
        set(board_index, cell, (pce_mch)Piece::Type::EMPTY);
    }
    

    void print_pieces(Index board_index, bool show_shift)
    {
        Coords::print_foreach(
        [board_index, show_shift](coord_mch cell)
        {
            pce_mch piece = get(board_index, cell);
            mserial_p(Piece::get_char(piece));

            if (!show_shift)
                return;
            else if (Piece::_get_type(piece)  == Piece::Type::EMPTY)
                mserial_p(' ');
            else if (Piece::_get_shift(piece) == Piece::Shift::TRUE)
                mserial_p('+');
            else
                mserial_p('-');
        },
        (show_shift) ? 2 : 1, true );
    }

    void print_cellnames()
    {
        Coords::print_foreach(
        [](coord_mch cell) { Coords::print_AN(cell); },
        Coords::RANK_LENGTH + 1, false );
    }
}