#include "core/utils.h"

#include "piece.h"
#include "board.h"
#include "game.h"
#include "mask.h"
#include "coords.h"

void setup()
{
    mserial_begin();

    mserial_pln(65/8);

    Game::reset();
}

void loop()
{
    Board::print_pieces(Board::MAJOR, true);
    Mask::print(Mask::FRONTLINE);

    #ifdef SERIAL_SPEED
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
    #endif
}
