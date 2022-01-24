#pragma once
#include "core/utils.h"

namespace Piece
{
    enum class Move : uint_mch
    {
        STEP       = 0b00000001,

        NONE       = 0 * STEP,
        INVALID    = (1 << 1) | NONE,

        CAPTURE    = (1 << 1) | STEP,
        DOUBLEPAWN = (2 << 1) | STEP,
        CASTLING   = (3 << 1) | STEP,
        EN_PASSANT = (4 << 1) | STEP,
        PROMOTION  = (5 << 1) | STEP
    };

    inline bool move_is_valid(Move move)
        { return (uint_mch)move & (uint_mch)Move::STEP; }
}