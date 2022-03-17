#include "piece.h"

#include "target.h"
#include "game.h"

namespace Piece
{
    char get_char(pce_mch piece)
    {
        if (is_empty(piece)) return EMPTY_CHAR;

        const auto convert = is_white(piece) ? ASCII::to_upper : ASCII::to_lower;
        return convert(ASCII::decode(piece));
    }

    bool _char_is_valid(char letter)
    {
        if (letter == EMPTY_CHAR) return true;
        for (char valid_char : VALID_CHARS)
            if (ASCII::to_upper(letter) == valid_char) return true;

        return false;
    }

    bool _promtype_is_valid(Type type)
    {
        for (Type prom_type : PROM_TYPES)
            if (type == prom_type) return true;
                
        return false;
    }


    pce_mch make(char letter, bool shift)
    {
        assert_val_mch(_char_is_valid(letter), letter);

        if (letter == EMPTY_CHAR)
            return (pce_mch)Type::EMPTY;
        else
            return make(_get_type(letter), reverse_color(_get_color(letter)), shift);
    }


    void _make_puremove(Board::Index board_index, coord_mch cell_from, coord_mch cell_to, Move move, Type prom_type)
    {
        assert_val_mch(Board::is_within(cell_from), cell_from,      DEC);
        assert_val_mch(Board::is_within(cell_to),   (uint_mch)move, DEC);

        const pce_mch piece_from    = Board::get(board_index, cell_from);
        const Color   color_from    = _get_color(piece_from);
        const bool    is_white_from = _is_white(color_from);

        switch (move)
        {
            // std:c++17 and later - attribute feature (a hint for compiler)
            // https://docs.microsoft.com/en-us/cpp/cpp/switch-statement-cpp
            case Move::STEP:        [[fallthrough]];
            case Move::DOUBLEPAWN:  [[fallthrough]];
            case Move::CAPTURE:
                Board::set(board_index, cell_to, make(_get_type(piece_from), color_from));
                Board::empty(board_index, cell_from);
                break;

            case Move::CASTLING:
                _make_puremove(board_index, cell_from, cell_to, Move::STEP);
                _make_puremove(board_index, _get_castling((cell_from > cell_to) ? L : R, is_white_from), (cell_from + cell_to)/2, Move::STEP);
                break;

            case Move::EN_PASSANT:
                _make_puremove(board_index, cell_from, cell_to, Move::STEP);
                Board::empty(board_index, _get_enpassant(cell_to, from_is_white));
                break;

            case Move::PROMOTION:
                assert_val_mch(_promtype_is_valid(type), (pce_mch)prom_type, DEC);

                Board::set(board_index, cell_to, make(prom_type, color_from));
                Board::empty(board_index, cell_from);
                break;

            default:
                assert_forced_val_mch((uint_mch)move, DEC);
        }
    }

    void _validate(coord_mch cell, coord_mch new_cell, Move move, bool full)
    {
        if (full) _make_puremove(Board::MINOR, cell, new_cell, move);

        for (coord_mch sub_cell = 0; sub_cell < Board::SIZE; sub_cell++)
            Piece::calculate(Board::MINOR, sub_cell);

        if (Board::king_is_hurt)
            Target::set(new_cell, Move::BLOCKED);
        else
        {
            Target::set(new_cell, move);
            Mask::set(cell, Mask::FRONTLINE);
        }
    }

    // magic happens here
    void calculate(Board::Index board_index, coord_mch cell)
    {
        const pce_mch piece       = Board::get(board_index, cell);
        const Type    type        = _get_type(piece);
        const Color   color       = _get_color(piece);
        const bool piece_is_white = _is_white(color);  // precomputed just because it appeares several times below

        if (board_index == Board::MAJOR)
            Target::reset();  // target is unique for every piece of <current_side> color

        // EMPTY cells are always ignored and only opponent's pieces are calculated on MINOR
        if (_is_empty(type) || (board_index == Board::MINOR && Game::is_current_side(color))) return;

        // PAWNs are analyzed separately because of their unique behaviour
        if (type == Type::PAWN)
        {
            //? #include <initializer_list> failed (perhaps, AVR compiler doesn't support this), so simply using const array
            const Dir pawn_rays[] = { (piece_is_white) ? UR : DR, (piece_is_white) ? UL : DL };  // PAWN CAPTURE/EN_PASSANT rays (forward diagonals)
            for (Dir ray : pawn_rays)
            {
                const coord_mch new_cell = cell + ray;
                if (!Board::is_within(cell, new_cell)) continue;

                const coord_mch new_piece = Board::get(board_index, new_cell);
                const Type      new_type  = _get_type(new_piece);
                const bool new_isnt_empty = !_is_empty(new_type);

                // when PAWN happens to stumble over piece of its color
                // (it can still be EMPTY in case of EN_PASSANT)
                if (new_isnt_empty && _get_color(new_piece) == color) continue;

                if (color == Game::current_side)
                {
                    Board::reset(Board::MINOR);

                    Move move;
                    if (new_isnt_empty)
                        move = Move::CAPTURE;
                    else if (Game::last_cell == _get_enpassant(new_cell, piece_is_white) && Game::last_move == Move::DOUBLEPAWN)
                        move = Move::EN_PASSANT;
                    else continue;

                    _full_validate(cell, new_cell, move);
                    
                }
                // we're only interested if opponent manages to capture the King
                // (at this point we're already sure it's of <current_side> color)
                else if (new_type == Type::KING)
                    _hurt_king(board_index, cell);
            }

            // no sense to calculate opponent's PAWNs as now we're looking only for forward non-CAPTURE steps
            if (!Game::is_current_side(color))
                return;

            const Dir ray = (piece_is_white) ? U : D;
            coord_mch new_cell = cell + ray;
            if (!( Board::is_within(cell, new_cell) && is_empty(Board::get(board_index, new_cell)) )) 
                return;

            // simulate this move on MINOR
            Board::reset(Board::MINOR);
            _full_validate(cell, new_cell, Move::STEP);

            // PAWNs can also make a double forwards step if it hasn't moved from starting position
            new_cell += ray;
            pce_mch new_piece = Board::get(board_index, new_cell);
            if (!( Board::is_within(cell, new_cell) && is_empty(new_piece) && !is_shifted(new_piece) ))
                return;

            Board::reset(Board::MINOR);
            return _full_validate(cell, new_cell, Move::DOUBLEPAWN);
        }

        // analyzing KING's CASTLING steps
        if (piece == make(Type::KING, Game::current_side, false))
        {
            const Dir castling_rays[] = {R, L};
            for (Dir ray : castling_rays)
            {
                // CASTLING is only possible if ROOK to swap with hasn't moved from starting position 
                const coord_mch rook_cell = _get_castling(ray, piece_is_white);
                if (Board::get(board_index, rook_cell) != make(Type::ROOK, Game::current_side, false))
                    continue;

                coord_mch new_cell = cell + ray;

               // simulate this move on MINOR
                Board::reset(Board::MINOR);
                for (int_mch king_steps = 0; (new_cell != rook_cell) && (is_empty(Board::get(board_index, new_cell))); king_steps++, new_cell += ray)
                { 
                    // cells that KING moves through must not be under opponent's attack
                    // and to simulate this we place KINGs on these cells on MINOR
                    // (actually, we place <piece> just because at this moment it so happens
                    // that <piece> contains KING piece of <current_side> color - and that's what we need)
                    //
                    // our original KING also remains on its <cell> on MINOR as it must not be under attack as well
                    if (king_steps < KING_CASTLING_MOVES) Board::set(Board::MINOR, new_cell, piece);
                }
                if (new_cell == rook_cell)
                    _validate(cell, cell + ray*2, Move::CASTLING);
            }
        }

        // a and b - start and end indices
        // for rays in < Piece::RAYS[] > to loop through
        int_mch a = 0, b = 0;
        switch (type)
        { 
            case Type::QUEEN:   a = 0; b = 2;  break;
            case Type::ROOK:    a = 0; b = 1;  break;
            case Type::BISHOP:  a = 1; b = 2;  break;
            case Type::KNIGHT:  a = 2; b = 4;  break;
            case Type::KING:    a = 0; b = 2;  break;
        }
        // loop over all different directions suitable for particular piece type
        for (int_mch ray_index = a*RAYS_ROW; ray_index < b*RAYS_ROW; ray_index++)
        {
            Dir ray = RAYS[ray_index];
            coord_mch new_cell = cell;

            for (coord_mch new_cell = cell+ray; Board::is_within(new_cell, new_cell-ray); new_cell += ray;)
            {
                const pce_mch new_piece = Board::get(board_index, new_cell);
                const Type new_type     = _get_type(new_piece);
                const bool new_is_empty = _is_empty(new_type);

                // if we step on cell occupied by piece of same color
                if (!new_is_empty && _get_color(new_piece) == color)
                    break;

                if (Game::is_current_side(color))
                {
                    // simulate this move on MINOR
                    Board::reset(Board::MINOR);
                    _full_validate(cell, new_cell, (new_is_empty) ? Move::STEP : Move::CAPTURE);
                }
                else if (new_type == Type::KING)
                    _hurt_king(board_index, cell);

                // no reason to check more than one step in any direction
                // if piece is of not-sliding type (e.g. KNIGHT)
                if (!(new_is_empty && is_sliding(piece)))
                    break;
            }
        }
    }
}