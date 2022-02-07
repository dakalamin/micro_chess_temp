#include "core/micro_assert.h"
#include "piece.h"

#include "target.h"
#include "game.h"

namespace Piece
{
    char get_char(pce_mch piece)
    {
        // 0b01CTTTTT - where C is Color, T is Type
        Type type = _get_type(piece);

        return (type == Type::EMPTY) ?
            EMPTY_CHAR : ((pce_mch)type | ((pce_mch)_get_color(piece) ^ (pce_mch)Color::WHITE) | 0b01000000);
    }


    void _make_puremove(Board::Index board_index, coord_mch cell_from, coord_mch cell_to, Move move, Type prom_type)
    {
        assert_val_mch(Board::is_within(cell_from), cell_from,      DEC);
        assert_val_mch(Board::is_within(cell_to),   (uint_mch)move, DEC);

        const pce_mch piece_from = Board::get(board_index, cell_from);
        const Color   color_from = _get_color(piece_from);
        const bool    is_white   = _is_white(color_from);

        switch (move)
        {
            case Move::STEP:
            case Move::DOUBLEPAWN:
            case Move::CAPTURE:
                Board::set(board_index, cell_to, (piece_from & (pce_mch)Prop::TC) | (pce_mch)Shift::TRUE);
                Board::empty(board_index, cell_from);
                return;

            case Move::CASTLING:
                _make_puremove(board_index, cell_from, cell_to, Move::STEP);
                _make_puremove(board_index, _get_castling((cell_from > cell_to) ? R : L, is_white), (cell_from + cell_to)/2, Move::STEP);
                return;

            case Move::EN_PASSANT:
                _make_puremove(board_index, cell_from, cell_to, Move::STEP);
                Board::empty(board_index, _get_enpassant(cell_to, is_white));
                return;

            case Move::PROMOTION:
                #ifdef DEBUG
                    bool promtype_is_valid = false;
                    for (Type type : PROM_TYPES)
                        promtype_is_valid += (prom_type == type);

                    assert_val_mch(promtype_is_valid, (pce_mch)prom_type, DEC);
                #endif

                Board::set(board_index, cell_to, (pce_mch)prom_type | (pce_mch)color_from | (pce_mch)Shift::TRUE);
                Board::empty(board_index, cell_from);
                return;

            default:
                assert_forced_val_mch((uint_mch)move, DEC);
                return;
        }
    }

    void _validate(coord_mch cell, coord_mch new_cell, Move move)
    {
        for (coord_mch sub_cell = 0; sub_cell < Board::SIZE; sub_cell++)
            Piece::calculate(Board::MINOR, sub_cell);

        if (Mask::king_is_hurt)
            Target::_set(new_cell, Move::INVALID);
        else
        {
            Target::_set(new_cell, move);
            Mask::set(cell, Mask::FRONTLINE);
        }
    }

    void _full_validate(coord_mch cell, coord_mch new_cell, Move move)
    {
        _make_puremove(Board::MINOR, cell, new_cell, move);
        _validate(cell, new_cell, move);
    }


    void calculate(Board::Index board_index, coord_mch cell)
    {
        const pce_mch  piece = Board::get(board_index, cell);
        const Type     type  = _get_type(piece);
        const Color    color = _get_color(piece);
        const bool  is_white = _is_white(color);

        if (board_index == Board::MAJOR)
        {
            Target::reset();
            Mask::reset(Mask::REGICIDES);
        }
        if (type == Type::EMPTY || (board_index == Board::MINOR && color == Game::current_side)) return;

        // a and b - start and end indices
        // for rays in 'const int_mch Piece::RAYS[]' to loop through
        int_mch a{0}, b{0};  bool is_sliding{false};
        switch (type)
        {
            case Type::ROOK:
                a = 0; b = 4;  is_sliding = true;
                break;
            case Type::BISHOP:
                a = 4; b = 8;  is_sliding = true;
                break;
            case Type::QUEEN:
                a = 0; b = 8;  is_sliding = true;
                break;

            case Type::KNIGHT:
                a = 8; b = 16;
                break;
            case Type::KING:
                a = 0; b = 8;
                break;
        }


        if (type == Type::PAWN)
        {
            // #include <initializer_list> failed (perhaps, AVR compiler doesn't support this), so simply using an array
            const Dir pawn_rays[] = { (is_white) ? UR : DR, (is_white) ? UL : DL };
            for (Dir ray : pawn_rays)
            {
                coord_mch new_cell = cell + ray;
                if (!Board::is_within(cell, new_cell)) continue;

                coord_mch new_piece = Board::get(board_index, new_cell);
                Type new_type = _get_type(new_piece);

                if (new_type != Type::EMPTY && _get_color(new_piece) == color) continue;

                if (color == Game::current_side)
                {
                    Board::reset(Board::MINOR);

                    Move move;
                    if (new_type != Type::EMPTY)
                        move = Move::CAPTURE;
                    else if (Game::last_cell == _get_enpassant(new_cell, is_white) && Game::last_move == Move::DOUBLEPAWN)
                        move = Move::EN_PASSANT;
                    else continue;

                    _full_validate(cell, new_cell, move);
                    
                }
                else if (new_type == Type::KING)
                    _hurt_king(board_index, cell);
            }

            if (color != Game::current_side)
                return;

            const Dir ray = (is_white) ? U : D;
            coord_mch new_cell = cell + ray;
            if (!(Board::is_within(cell, new_cell) && _get_type(Board::get(board_index, new_cell)) == Type::EMPTY)) 
                return;

            Board::reset(Board::MINOR);
            _full_validate(cell, new_cell, Move::STEP);

            new_cell += ray;
            pce_mch new_piece = Board::get(board_index, new_cell);
            if (!(Board::is_within(cell, new_cell) && _get_type(new_piece) == Type::EMPTY && _get_shift(new_piece) == Shift::FALSE))
                return;

            Board::reset(Board::MINOR);
            _full_validate(cell, new_cell, Move::DOUBLEPAWN);

            return;
        }

        else if (piece == ((pce_mch)Type::KING | (pce_mch)Game::current_side | (pce_mch)Shift::FALSE))
        {
            Dir castling_rays[] = {R, L};

            for (Dir ray : castling_rays)
            {
                coord_mch rook_cell = _get_castling(ray, is_white);
                if (Board::get(board_index, rook_cell) != ((pce_mch)Type::ROOK | (pce_mch)Game::current_side | (pce_mch)Shift::FALSE))
                    continue;

                coord_mch new_cell = cell + ray;

                Board::reset(Board::MINOR);
                for (int_mch counter = 0; new_cell != rook_cell && _get_type(Board::get(board_index, new_cell)) == Type::EMPTY; counter++, new_cell += ray)
                {
                    if (counter < 2) Board::set(Board::MINOR, new_cell, piece);
                }
                if (new_cell == rook_cell)
                    _validate(cell, cell + ray*2, Move::CASTLING);
            }
        }

        // loop over all different directions suitable for the particular piece type
        for (int_mch ray_index = a; ray_index < b; ray_index++)
        {
            Dir ray = RAYS[ray_index];
            coord_mch new_cell = cell;

            while (Board::is_within(new_cell, new_cell+ray))
            {
                new_cell += ray;
                pce_mch new_piece = Board::get(board_index, new_cell);
                Type     new_type = _get_type(new_piece);

                bool new_is_empty = new_type == Type::EMPTY;
                if (!new_is_empty && _get_color(new_piece) == color) break;

                if (color == Game::current_side)
                {
                    Board::reset(Board::MINOR);
                    _full_validate(cell, new_cell, (new_is_empty) ? Move::STEP : Move::CAPTURE);
                }
                // if the opponent manages to capture the King
                else if (new_type == Type::KING)
                    _hurt_king(board_index, cell);

                // no reason to check more than one step in any direction
                // if the piece is of 'not-sliding' type (e.g. Knight)
                if (!(is_sliding && new_is_empty))
                    break;
            }
        }
    }
}