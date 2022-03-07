#pragma once
#include "core/utils.h"

namespace Piece
{
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
    inline char move2char(Move move)
        { return ASCII::to_upper(ASCII::decode(move)); }
}