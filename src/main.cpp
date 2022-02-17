#include <Arduino.h>

#include "core/utils.h"
#include "core/micro_assert.h"
#include "core/micro_serial.h"

#include "board.h"
#include "game.h"
#include "mask.h"


void setup()
{
    mserial_begin();

    Game::reset();
    
    #ifdef DEBUG
        Board::fill(Board::MAJOR, Board::BRD_3);
        Game::preanalyze();
    #endif

    Board::print_cellnames();
    Board::print_pieces(Board::MAJOR);

    Mask::print(Mask::FRONTLINE);

    Game::analyze_input(62);
    Target::print();
}

void loop()
{
    /*while (!Serial.available())
        delay(10);

    char buf[2] = {};
    Serial.readBytes(buf, 4);

    Serial.print(buf[0]);
    Serial.print(' ');
    Serial.println(buf[1]);

    Serial.find('\0');*/
}
