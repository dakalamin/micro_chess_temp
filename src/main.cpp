#include <Arduino.h>

#include "core/utils.h"
#include "core/micro_assert.h"

#include "board.h"
#include "game.h"
#include "mask.h"
#include "piece.h"

#include "test.h"

void setup()
{
    Serial.begin(SERIAL_SPEED);

    Game::reset(); 
    Serial.println(F("CELLNAMES"));
    Board::print_cellnames();
    Serial.println(F("STARTING POSITION"));
    Board::print_pieces(Board::MAJOR);
    Serial.println(F("STARTING FRONTLINE"));
    Mask::print(Mask::FRONTLINE);

    coord_mch piece = Board::SIZE-1 + Piece::U + Piece::L * 4;
    Game::analyze_input(piece);
    Serial.println(F("\nPOSSIBLE STEPS"));
    Target::print();

    Serial.println(F("\n--- MAKING THE MOVE ---\n"));
    Game::make_move(piece, piece + Piece::U * 2);
    Serial.println(F("POSITION DEPTH 1"));
    Board::print_pieces(Board::MAJOR);
    Serial.println(F("FRONTLINE DEPTH 1"));
    Mask::print(Mask::FRONTLINE);

    piece = Piece::R;
    Game::analyze_input(piece);
    Serial.println(F("\nPOSSIBLE STEPS DEPTH 1"));
    Target::print();
}

void loop() { }