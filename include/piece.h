#pragma once
#include "core/utils.h"
#include "core/micro_serial.h"

#include "move.h"
#include "board.h"
#include "mask.h"

namespace Piece
{
    enum class Prop : pce_mch
    {
        TYPE  = (pce_mch)ASCII::Bits::NOCASE_CHAR,
        COLOR = (pce_mch)ASCII::Bits::LOWERCASE,
        SHIFT = COLOR << 1,

        TC    = TYPE | COLOR
    };

    const char VALID_CHARS[] = "KQRNBP";
    const char EMPTY_CHAR = '.';  // That is printed for EMPTY cells

    enum class Type : pce_mch
    {
        EMPTY  = 0,
        KING   = ASCII::encode('K'),
        QUEEN  = ASCII::encode('Q'),
        ROOK   = ASCII::encode('R'),
        KNIGHT = ASCII::encode('N'),
        BISHOP = ASCII::encode('B'),
        PAWN   = ASCII::encode('P')
    };
    const Type PROM_TYPES[] =  // Types that KING can be promoted to
    { Type::QUEEN, Type::ROOK, Type::KNIGHT, Type::BISHOP };

    // During CASTLING either long (left) or short one (right)
    // KING moves exactly 2 cells in that direction
    const int_mch KING_CASTLING_MOVES = 2;

    enum class Color : pce_mch
    {
        BLACK = 0,
        WHITE = (pce_mch)Prop::COLOR
    };

    enum class Shift : pce_mch
    {
        FALSE = 0,
        TRUE  = (pce_mch)Prop::SHIFT
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
    const int RAYS_ROW = 4;
    const Dir RAYS[RAYS_ROW*4] =
    {
        R,   D,   L,   U,    // Horizontal & Vertical
        DR,  DL,  UR,  UL,   // Diagonal
        DLL, DRR, DDL, DDR,  // Knight steps
        ULL, URR, UUL, UUR   // ...
    };


    bool _char_is_valid(char letter);
    char get_char(pce_mch piece);

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
        { Mask::set(cell, (Mask::_king_is_hurt = (board_index == Board::MINOR)) ? Mask::MENACES : Mask::REGICIDES); }

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