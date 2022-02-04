#include <Arduino.h>

#include "core/utils.h"
#include "core/micro_assert.h"

#include "board.h"
#include "game.h"
#include "mask.h"
#include "piece.h"

void setup()
{
    Serial.begin(SERIAL_SPEED);
    while (!Serial) { }

    Board::print_cellnames();
    Board::print_cellcoords();

    Game::reset();
}

void loop()
{
    Game::preanalyze();

    Board::print(Board::MAJOR);
    Serial.println();

    if (Game::has_ended())
    {
        if (Game::current_state == Game::State::DRAW)
            Serial.println(F("That's a DRAW, nobody wins"));
        else
        {
            Piece::print_color(Game::current_side);
            Serial.println(F(" has won, congratulations"));
        }

        Serial.println(F("\n --- RESTARTING THE GAME --- \n"));
        return Game::reset();
    }

    Game::print_side();
}
