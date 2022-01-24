#include <Arduino.h>

#include "core/micro_assert.h"
#include "board.h"

#include "game.h"
#include "mask.h"
#include "piece.h"
#include "target.h"

namespace Board
{
    pce_mch arr[Index::arr_size][SIZE];


    void fill(Index board_index, const char contents[])
    {
        using namespace Piece;
        for (coord_mch cell = 0; cell < SIZE; cell++)
        {
            char new_piece = contents[cell];

            #ifdef DEBUG
                int_mch brd_size;
                for (brd_size = 0; contents[brd_size] != '\0'; brd_size++) { }
                assert_val_mch(brd_size == Board::SIZE, brd_size);

                bool valid = (new_piece == EMPTY_CHAR);
                for (int_mch i = 0; !valid && (i < NUM_TYPES); i++)
                    valid = ((new_piece | (char)Prop::COLOR) == VALID_CHARS[i]);

                assert_val_mch(valid, new_piece);
            #endif

            pce_mch piece = (pce_mch)Type::EMPTY;;
            Shift shift   = Shift::FALSE;

            if (new_piece != EMPTY_CHAR)
            {
                piece  = (pce_mch)get_type((pce_mch)new_piece);
                piece |= ((pce_mch)get_color((pce_mch)new_piece) ^ (pce_mch)Piece::Color::WHITE);

                if (!(contents == BRD_DEFAULT || new_piece == BRD_DEFAULT[cell]))
                    shift = Shift::TRUE;
            }

            get(board_index)[cell] = piece | (pce_mch)shift;
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
                    get(MINOR)[cell] = get(MAJOR)[cell];

                Mask::king_is_hurt = false;
                return;
                
            default:
                assert_val_mch(0, board_index, DEC);
                return;
        }
    }

    void empty(Index board_index, coord_mch cell)
    {
        get(board_index)[cell] = (pce_mch)Piece::Type::EMPTY;
    }


    template <typename Func>
    void print(Func function)
    {
        for (coord_mch row = 0; row < SIDE; row++)
        {
            for (coord_mch column = 0; column < SIDE; column++)
            {
                char a, b;
                function(row*SIDE + column, a, b);

                Serial.print(a); Serial.print(b); Serial.print(' ');
            }
            Serial.println(); 
        }
        Serial.println();
    }

    void print_pieces(Index board_index)
    {
        using namespace Piece;
        print([board_index](coord_mch cell, char& a, char& b)
        {
            pce_mch piece = Board::get(board_index)[cell];
            a = get_char(piece);

            if (get_type(piece) == Type::EMPTY)
                b = ' ';
            else if (get_shift(piece) == Shift::TRUE)
                b = '+';
            else
                b = '-';
        });
    }

    void print_cellnames()
    {
        print([](coord_mch cell, char& rank, char& file)
        {
            rank = '8' - cell / SIDE;
            file = 'a' + cell % SIDE;
        });
    }
}