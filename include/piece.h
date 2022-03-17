#pragma once
#include "core/utils.h"

#include "move.h"
#include "board.h"
#include "mask.h"

namespace Piece
{
    enum class Prop : pce_mch
    {
        SLIDE = LEFTMOST_BIT,

        TYPE  = (pce_mch)ASCII::Bits::NOCASE | SLIDE,
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
        QUEEN  = ASCII::encode('Q') | (pce_mch)Prop::SLIDE,
        ROOK   = ASCII::encode('R') | (pce_mch)Prop::SLIDE,
        KNIGHT = ASCII::encode('N'),
        BISHOP = ASCII::encode('B') | (pce_mch)Prop::SLIDE,
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


    enum Dir: int_mch
    {
        R   = 1,
        D   = Board::WIDTH,
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
    bool _promtype_is_valid(Type type);
    char get_char(pce_mch piece);

           pce_mch make(char letter, bool shift=true);
    inline pce_mch make(Type type, Color color, bool shift=true)
        { return (pce_mch)type | (pce_mch)color | (shift * (pce_mch)Prop::SHIFT); }
    inline pce_mch get_prop(pce_mch piece, Prop prop)
        { return piece & (pce_mch)prop; }

    inline Type  _get_type (pce_mch piece) { return (Type)get_prop(piece,  Prop::TYPE);  }
    inline Color _get_color(pce_mch piece) { return (Color)get_prop(piece, Prop::COLOR); }
    inline bool _is_empty(Type type)       { return type  == Type::EMPTY;  }
    inline bool _is_white(Color color)     { return color == Color::WHITE; }

    inline bool is_sliding(pce_mch piece)  { return get_prop(piece, Prop::SLIDE); } 
    inline bool is_shifted(pce_mch piece)  { return get_prop(piece, Prop::SHIFT); }
    inline bool is_empty(pce_mch piece)    { return _is_empty(_get_type(piece));  }
    inline bool is_white(pce_mch piece)    { return _is_white(_get_color(piece)); }

    inline Color reverse_color(Color color)
        { return _is_white(color) ? Color::BLACK : Color::WHITE; }

    inline void _hurt_king(Board::Index board_index, coord_mch cell)
        { Mask::set(cell, (Board::king_is_hurt = (board_index == Board::MINOR)) ? Mask::MENACES : Mask::REGICIDES); }

    inline coord_mch _get_enpassant(coord_mch cell, bool piece_is_white)
        { return cell + (piece_is_white) ? D : U; }
    inline coord_mch _get_castling(Dir ray, bool is_white)
        { return (Board::WIDTH-1) * (ray == R) + (Board::SIZE-Board::WIDTH) * is_white; }

    void _make_puremove(Board::Index board_index, coord_mch cell_from, coord_mch cell_to, Move move, Type prom_type=Type::EMPTY);
    void _validate(coord_mch cell, coord_mch new_cell, Move move, bool full=false);
    void calculate(Board::Index board_index, coord_mch cell);

#ifdef SERIAL_SPEED
    inline void print_color(Color color) { mserial_p(_is_white(color) ? F("WHITE") : F("BLACK")); }
#else
    inline void print_color(Color color) { }
#endif
}