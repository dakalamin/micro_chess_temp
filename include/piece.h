#pragma once
#include "core/utils.h"
#include "core/micro_serial.h"

#include "move.h"
#include "board.h"
#include "mask.h"

namespace Piece
{
    #ifdef DEBUG
        const char VALID_CHARS[] = "kqrnbp";  // KEEP IN LOWER CASE!
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
        MASK  = (pce_mch)Type::MASK + 1,

        BLACK = 0 * MASK,
        WHITE = 1 * MASK
    };

    enum class Shift : pce_mch
    {
        MASK  = (pce_mch)Color::MASK << 1,

        FALSE = 0 * MASK,
        TRUE  = 1 * MASK
    };

    enum class Prop : pce_mch
    {
        TYPE  = (pce_mch)Type::MASK,
        COLOR = (pce_mch)Color::MASK,
        SHIFT = (pce_mch)Shift::MASK,

        TC    = TYPE | COLOR
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


    char get_char (pce_mch piece);

    inline pce_mch get_prop(pce_mch piece, Prop prop)
        { return piece & (pce_mch)prop; }
    inline Type  _get_type (pce_mch piece)
        { return (Type)get_prop(piece, Prop::TYPE); }
    inline Color _get_color(pce_mch piece)
        { return (Color)get_prop(piece, Prop::COLOR); }
    inline Shift _get_shift(pce_mch piece)
        { return (Shift)get_prop(piece, Prop::SHIFT); }

    inline bool _is_white(Color color)
        { return color == Color::WHITE; }
    inline Color opposite_color(Color color)
        { return  _is_white(color) ? Color::BLACK : Color::WHITE; }

    inline void _hurt_king(Board::Index board_index, coord_mch cell)
        { Mask::set(cell, (Mask::king_is_hurt = (board_index == Board::MINOR)) ? Mask::MENACES : Mask::REGICIDES); }

    inline coord_mch _get_enpassant(coord_mch cell, bool is_white)
        { return cell + (is_white) ? D : U; }
    inline coord_mch _get_castling(Dir ray, bool is_white)
        { return (Board::SIDE-1) * (ray == R) + (Board::SIZE-Board::SIDE) * is_white; }

    void _make_puremove(Board::Index board_index, coord_mch cell_from, coord_mch cell_to, Move move, Type prom_type=Type::EMPTY);

    void _validate(coord_mch cell, coord_mch new_cell, Move move);
    void _full_validate(coord_mch cell, coord_mch new_cell, Move move);

    void calculate(Board::Index board_index, coord_mch cell);

    inline void print_color(Color color)
        { mserial_p(_is_white(color) ? F("WHITE") : F("BLACK")); }
}