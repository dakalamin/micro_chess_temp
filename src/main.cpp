#include "core/utils.h"

#include "board.h"
#include "game.h"
#include "mask.h"
#include "coords.h"

char buf[2];

void setup()
{
    mserial_begin();
    Game::reset();
}

void loop()
{
    Board::print_pieces(Board::MAJOR);
    Mask::print(Mask::FRONTLINE);

    coord_mch cell_from, cell_to;
    while (true)
    {
        while (true)
        {
            mserial_pln("Choose a cell of a piece to move");
            cell_from = Coords::read_and_parse();
            if (Mask::check(cell_from, Mask::FRONTLINE)) break;

            mserial_pln("Not a valid choice, please try again");
        }
        Game::analyze_input(cell_from);
        Target::print();

        cell_to = Coords::read_and_parse();
        mserial_pln("Choose a cell to move to");
        if (!Piece::move_is_invalid(Target::get(cell_to))) break;

        mserial_pln("Not a valid choice, you'll be returned to a piece choice");
    }

    Game::make_move(cell_from, cell_to);
}
