#include <Arduino.h>
#include "game.h"
#include "board.h"

namespace Game
{
    Piece::Color current_side;
    State        current_state;

    Piece::Move last_move;
    coord_mch   last_cell;

    void print_side()
    {
        Piece::print_color(current_side);
        Serial.println(F(" side is playing now"));
    }

    void print_state()
    {
        switch (current_state)
        {
            case State::CHECKMATE:
                Serial.print(F("CHECKMATE")); break;
            case State::STALEMATE:
                Serial.print(F("STALEMATE")); break;
            case State::DRAW:
                Serial.print(F("DRAW")); break;
        }
    }

    void reset(Piece::Color start_side)
    {
        current_state = State::NEUTRAL;
        current_side  = start_side;

        last_move = Piece::Move::NONE;
        last_cell = -1;

        Board::reset(Board::MAJOR);
        preanalyze();
    }


    void preanalyze()
    {
        Mask::reset(Mask::FRONTLINE);
        Mask::reset(Mask::REGICIDES);

        for (coord_mch cell = 0; cell < Board::SIZE; cell++)
            Piece::calculate(Board::MAJOR, cell);

        using namespace Game;
        if (Mask::get(Mask::FRONTLINE))
            current_state = (Mask::get(Mask::REGICIDES)) ? State::CHECK : State::NEUTRAL;
        else
            current_state = (Mask::get(Mask::REGICIDES)) ? State::CHECKMATE : State::STALEMATE;
    }

    void analyze_input(coord_mch cell)  // check the piece chosen to step
    {
        Piece::calculate(Board::MAJOR, cell);
    }

    void _make_move(coord_mch cell_from, coord_mch cell_to, Piece::Move move, Piece::Type prom_type)
    {
        switch_sides();
        last_cell = cell_to;
        last_move = move;

        Piece::_make_puremove(Board::MAJOR, cell_from, cell_to, move, prom_type);
    }

    void make_move(coord_mch cell_from, coord_mch cell_to, Piece::Type prom_type)
    {
        Piece::Move move = Target::get(cell_to);
        if (!Piece::move_is_valid(move)) return;

        _make_move(cell_from, cell_to, move, prom_type);
        Game::preanalyze();
    }
}