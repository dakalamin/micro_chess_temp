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
        _END      = DFLT_MASK,

        NEUTRAL   = 0 * _END,
        CHECK     = 1 | NEUTRAL,  // ШАХ

        CHECKMATE = 2 | _END,     // МАТ
        STALEMATE = 3 | _END,     // ПАТ
        DRAW      = 4 | _END      // НИЧЬЯ
    };
    extern State current_state;

    extern Piece::Move last_move;
    extern coord_mch   last_cell;


    inline bool has_ended()
        { return (uint_mch)current_state & (uint_mch)State::_END; }
    inline void switch_sides()
        { current_side = Piece::opposite_color(current_side); }

    void print_state();
    void print_side();
    void reset(Piece::Color start_side=START_SIDE);

    void preanalyze();
    void analyze_input(coord_mch cell);
    void _make_move(coord_mch cell_from, coord_mch cell_to, Piece::Move move, Piece::Type prom_type=Piece::Type::EMPTY);
    void make_move(coord_mch cell_from, coord_mch cell_to, Piece::Type prom_type=Piece::Type::EMPTY);
}