#pragma once
#include "core/utils.h"

#include "piece.h"
#include "move.h"
#include "target.h"

namespace Game
{
    const  Piece::Color START_SIDE = Piece::Color::WHITE;
    extern Piece::Color current_side;

    enum class State : uint_mch
    {
        NEUTRAL,
        CHECK,      // ШАХ
        CHECKMATE,  // МАТ
        STALEMATE,  // ПАТ
        DRAW        // НИЧЬЯ
    };
    extern State current_state;

    extern Piece::Move last_move;
    extern coord_mch   last_cell;


    inline Piece::Color opp_side(Piece::Color side)
        { return (side == Piece::Color::WHITE) ? Piece::Color::BLACK : Piece::Color::WHITE; }
    inline void switch_side()
        { current_side = opp_side(current_side); }

    void reset(Piece::Color start_side=START_SIDE);

    void preanalyze();
    void analyze_input(coord_mch cell);
    void make_move(coord_mch cell_from, coord_mch cell_to, Piece::Move move, Piece::Type prom_type=Piece::Type::EMPTY);
    void make_move(coord_mch cell_from, coord_mch cell_to, Piece::Type prom_type=Piece::Type::EMPTY);
}