#include "core/micro_assert.h"
#include "core/micro_serial.h"
#include "game.h"

namespace Game
{
    Piece::Color current_side;
    State        current_state;

    count_mch    turns_counter;

    Piece::Move  last_move;
    coord_mch    last_cell;


    void reset(Piece::Color start_side)
    {
        current_state = State::NEUTRAL;
        current_side  = start_side;

        turns_counter = 0;

        last_move = Piece::Move::NONE;
        last_cell = -1;

        Board::reset(Board::MAJOR);
        preanalyze();
    }


    void print_state()
    {
        if (has_ended())
            mserial_plns("Game ends at ");
        else if (current_state == State::CHECK)
            mserial_plns("Be careful, it's CHECK");


        switch (current_state)
        {
            case State::CHECKMATE:
                mserial_ps("CHECKMATE\nCongratulations ");
                Piece::print_color(Piece::opposite_color(current_side));
                mserial_plns(" side, you win!");
                break;
            case State::STALEMATE:
                mserial_plns("STALEMATE");  break;
            case State::DRAW:
                mserial_plns("DRAW");       break;
        }

        if (has_ended() && current_state != State::CHECKMATE)
            mserial_plns("Nobody wins!");

    }

    void print_side()
    {
        Piece::print_color(current_side);
        mserial_plns(" are playing now\n");
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

        print_state();
        if (has_ended())
        {
            if (turns_counter == 0)
            {
                mserial_plns("Starting configuration seems to be an example of end-game state");
                mserial_plns("Game won't restart automatically to prevent going into infinite loop...");
            }

            mserial_plns("Now game restarts...");
            Game::reset();
        }
        else
        {
            if (turns_counter != 0)
                mserial_ps("Game continues... ");
                
            print_side();
        }
    }
    
    void _make_move(coord_mch cell_from, coord_mch cell_to, Piece::Move move, Piece::Type prom_type)
    {
        switch_sides();
        turns_counter++;

        last_cell = cell_to;
        last_move = move;

        Piece::_make_puremove(Board::MAJOR, cell_from, cell_to, move, prom_type);
    }

    void make_move(coord_mch cell_from, coord_mch cell_to, Piece::Type prom_type)
    {
        Piece::Move move = Target::get(cell_to);
        if (Piece::move_is_invalid(move)) return;

        _make_move(cell_from, cell_to, move, prom_type);
        Game::preanalyze();
    }
}