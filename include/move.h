#pragma once
#include "core/utils.h"

namespace Piece
{
    enum class Move : uint_mch
    {
        STEP       = DFLT_MASK,

        NONE       = 0 * STEP,
        INVALID    = 1 | NONE,

        CAPTURE    = 2 | STEP,
        DOUBLEPAWN = 3 | STEP,
        CASTLING   = 4 | STEP,
        EN_PASSANT = 5 | STEP,
        PROMOTION  = 6 | STEP
    };

    inline bool move_is_valid(Move move)
        { return (uint_mch)move & (uint_mch)Move::STEP; }
}