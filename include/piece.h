#pragma once
#include <Arduino.h>
#include "core/utils.h"
#include "target.h"

#include "board.h"
#include "mask.h"

namespace Piece
{
    #ifdef DEBUG
        const char VALID_CHARS[] = "kqrnbp";  // KEEP IN LOWER CASE!
        const int_mch NUM_TYPES = sizeof(VALID_CHARS)/sizeof(VALID_CHARS[0]);
    #endif
    const char EMPTY_CHAR = '.';
    enum class Type : pce_mch
    {
        MASK   = 0b00011111,

        EMPTY  =  0  & MASK,
        KING   = 'K' & MASK,  // 010 0b01011 11 -
        QUEEN  = 'Q' & MASK,  // 010 0b10001 17 s
        ROOK   = 'R' & MASK,  // 010 0b10010 18 s
        KNIGHT = 'N' & MASK,  // 010 0b01110 14 -
        BISHOP = 'B' & MASK,  // 010 0b00010  2 s
        PAWN   = 'P' & MASK   // 010 0b10000 16 -
    };
    const Type PROM_TYPES[] =
    { Type::QUEEN, Type::ROOK, Type::KNIGHT, Type::BISHOP };

    enum class Color : pce_mch
    {
        MASK  = 0b00100000,

        BLACK = 0 * MASK,
        WHITE = 1 * MASK
    };

    enum class Shift : pce_mch
    {
        MASK  = 0b01000000,

        FALSE = 0 * MASK,
        TRUE  = 1 * MASK
    };

    enum class Prop : pce_mch
    {
        TYPE  = (pce_mch)Type::MASK,
        COLOR = (pce_mch)Color::MASK,
        SHIFT = (pce_mch)Shift::MASK,

        TCS   = TYPE | COLOR | SHIFT,
        TC    = TYPE | COLOR,
    };


    enum Dir: int_mch
    {
        R   = 1,
        D   = Board::SIDE,
        L   = -R,
        U   = -D,
        DR  = D+R,  UL  = -DR,
        DL  = D+L,  UR  = -DL,
        DLL = DL+L, DRR = DR+R, DDL = D+DL, DDR = D+DR,
        ULL = UL+L, URR = UR+R, UUL = U+UL, UUR = U+UR
    };
    const Dir RAYS[] =
    {
        R,   D,   L,   U,    // Horizontal & Vertical
        DR,  DL,  UR,  UL,   // Diagonal
        DLL, DRR, DDL, DDR,  // Knight steps
        ULL, URR, UUL, UUR   // ...
    };


    inline pce_mch get_prop(pce_mch piece, Prop prop)
        { return piece & (pce_mch)prop; }

    char get_char (pce_mch piece);
    inline Type  get_type (pce_mch piece)
        { return (Type)get_prop(piece, Prop::TYPE); }
    inline Color get_color(pce_mch piece)
        { return (Color)get_prop(piece, Prop::COLOR); }
    inline Shift get_shift(pce_mch piece)
        { return (Shift)get_prop(piece, Prop::SHIFT); }

    inline bool _is_white(Color color)
        { return color == Color::WHITE; }
    inline Color opposite_color(Color color)
        { return  _is_white(color) ? Color::BLACK : Color::WHITE; }

    inline void king_hurt(Board::Index board_index, coord_mch cell)
        { Mask::append(cell, (Mask::king_is_hurt = (board_index == Board::MINOR)) ? Mask::MENACES : Mask::REGICIDES); }

    coord_mch get_enpassant(coord_mch cell);
    coord_mch get_castling(Dir ray);

    void _make_move(Board::Index board_index, coord_mch cell_from, coord_mch cell_to, Move move, Type prom_type=Type::EMPTY);

    void _validate(coord_mch cell, coord_mch new_cell, Move move);
    void _full_validate(coord_mch cell, coord_mch new_cell, Move move);

    void calculate(Board::Index board_index, coord_mch cell);
}