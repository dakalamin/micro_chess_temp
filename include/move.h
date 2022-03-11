#pragma once
#include "core/utils.h"

namespace Piece
{
    const char INVALID_CHAR = 'o';
    enum class Move : uint_mch
    {
        _INVALID   = LEFTMOST_BIT,

        STEP       = ASCII::encode('S'),
        CAPTURE    = ASCII::encode('A'),
        DOUBLEPAWN = ASCII::encode('D'),
        CASTLING   = ASCII::encode('C'),
        EN_PASSANT = ASCII::encode('E'),
        PROMOTION  = ASCII::encode('P'),

        NONE       = ASCII::encode('N') | _INVALID,
        BLOCKED    = ASCII::encode('I') | _INVALID
    };

    inline bool move_is_invalid(Move move)
        { return (uint_mch)move & (uint_mch)Move::_INVALID; }
    inline char convert_move2char(Move move, bool suppress_invalid=false)
        { return (suppress_invalid && move_is_invalid(move)) ?
                  INVALID_CHAR : ASCII::to_upper(ASCII::decode(move));
        }
}