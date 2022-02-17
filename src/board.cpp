#include "core/micro_assert.h"
#include "core/micro_serial.h"
#include "board.h"

#include "game.h"
#include "mask.h"
#include "piece.h"
#include "target.h"

namespace Board
{
    pce_mch _arr[Index::_arr_size][SIZE];


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


    void _print(auto function)
    {
        for (coord_mch row = 0; row < SIDE; row++)
        {
            for (coord_mch column = 0; column < SIDE; column++)
            {
                char a, b;
                function(row*SIDE + column, a, b);

                mserial_p(a); mserial_p(b); mserial_p(' ');
            }
            mserial_pln(); 
        }
        mserial_pln();
    }

    void print_pieces(Index board_index)
    {
        _print([board_index](coord_mch cell, char& piece_char, char& piece_shift)
        {
            pce_mch piece = get(board_index, cell);
            piece_char    = Piece::get_char(piece);

            if (Piece::_get_type(piece) == Piece::Type::EMPTY)
                piece_shift = ' ';
            else if (Piece::_get_shift(piece) == Piece::Shift::TRUE)
                piece_shift = '+';
            else
                piece_shift = '-';
        });
    }

    void print_cellnames()
    {
        _print([](coord_mch cell, char& rank, char& file)
        {
            rank = '8' - cell / SIDE;
            file = 'a' + cell % SIDE;
        });
    }
}