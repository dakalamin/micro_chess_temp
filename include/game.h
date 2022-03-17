#pragma once
#include "core/utils.h"

#include "piece.h"
#include "move.h"
#include "board.h"

namespace Game
{
    const  Piece::Color START_SIDE = Piece::Color::WHITE;
    extern Piece::Color current_side;

    enum class State : uint_mch
    {
        _END      = LEFTMOST_BIT,

        NEUTRAL   = ASCII::encode('N'),
        CHECK     = ASCII::encode('C'),

        CHECKMATE = ASCII::encode('M') | _END,
        STALEMATE = ASCII::encode('S') | _END,
        DRAW      = ASCII::encode('D') | _END
    };
    extern State current_state;

    extern count_mch   turns_counter;
    extern Piece::Move last_move;
    extern coord_mch   last_cell;


    inline bool is_current_side(Piece::Color color)
        { return color == current_side; };
    inline bool has_ended()
        { return (uint_mch)current_state & (uint_mch)State::_END; }
    inline void switch_sides()
        { current_side = Piece::reverse_color(current_side); }
    void reset(Piece::Color start_side=START_SIDE);

    void print_side();   // can be used independently for debugging
    void print_state();  // can be used independently for debugging

    inline void analyze_input(coord_mch cell) { Piece::calculate(Board::MAJOR, cell); }
        
    void preanalyze();
    void _make_move(coord_mch cell_from, coord_mch cell_to, Piece::Move move, Piece::Type prom_type=Piece::Type::EMPTY);
    void  make_move(coord_mch cell_from, coord_mch cell_to,                   Piece::Type prom_type=Piece::Type::EMPTY);
}