#include <Arduino.h>

#include "core/utils.h"
#include "core/micro_assert.h"

#include "board.h"
#include "game.h"
#include "mask.h"

void setup()
{
    Serial.begin(SERIAL_SPEED);
    while (!Serial) { }
    Serial.println();

    Game::reset();
    #ifdef DEBUG
        Board::fill(Board::MAJOR, Board::BRD_2);
        Game::preanalyze();
    #endif

    Board::print_cellnames();
    Board::print_pieces(Board::MAJOR);

    Mask::print(Mask::FRONTLINE);

    Game::analyze_input(34);
    Target::print();
}

void loop()
{ }
