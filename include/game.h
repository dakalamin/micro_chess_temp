#pragma once
#include "core/utils.h"

#include "piece.h"
#include "move.h"
#include "target.h"
#include "board.h"

namespace Game
{
    const  Piece::Color START_SIDE = Piece::Color::WHITE;
    extern Piece::Color current_side;

    enum class State : uint_mch
    {
        _END      = DFLT_MASK_MCH,

        NEUTRAL   = ASCII::encode('N'),         // ПРОДОЛЖЕНИЕ ПАРТИИ
        CHECK     = ASCII::encode('C'),         // ШАХ

        CHECKMATE = ASCII::encode('M') | _END,  // МАТ
        STALEMATE = ASCII::encode('S') | _END,  // ПАТ
        DRAW      = ASCII::encode('D') | _END   // НИЧЬЯ
    };
    extern State current_state;

    extern count_mch   turns_counter;
    extern Piece::Move last_move;
    extern coord_mch   last_cell;


    inline bool has_ended()
        { return (uint_mch)current_state & (uint_mch)State::_END; }
    inline void switch_sides()
        { current_side = Piece::opposite_color(current_side); }
    void reset(Piece::Color start_side=START_SIDE);

    void print_side();
    void print_state();

    inline void analyze_input(coord_mch cell)
        { Piece::calculate(Board::MAJOR, cell); }
    void preanalyze();
    void _make_move(coord_mch cell_from, coord_mch cell_to, Piece::Move move, Piece::Type prom_type=Piece::Type::EMPTY);
    void make_move(coord_mch cell_from, coord_mch cell_to, Piece::Type prom_type=Piece::Type::EMPTY);
}