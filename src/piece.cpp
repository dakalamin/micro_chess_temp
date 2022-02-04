#include "core/micro_assert.h"
#include "piece.h"

#include "board.h"
#include "game.h"
#include "mask.h"

namespace Piece
{
    char get_char(pce_mch piece)
    {
        // 0b01CTTTTT - where C is Color, T is Type
        Type type = get_type(piece);

        return (type == Type::EMPTY) ?
            EMPTY_CHAR : ((pce_mch)type | ((pce_mch)get_color(piece) ^ (pce_mch)Color::WHITE) | 0b01000000);
    }

    coord_mch get_enpassant(coord_mch cell)
    {
        return cell + D*((Game::current_side == Game::START_SIDE) ? 1 : -1);
    }

    coord_mch get_castling(Piece::Dir ray)
    {
        return (Board::SIDE-1)*(ray == R) + (Board::SIZE-Board::SIDE)*(Game::current_side == Game::START_SIDE);
    }


    void _make_move(Board::Index board_index, coord_mch cell_from, coord_mch cell_to, Move move, Type prom_type)
    {
        assert_val_mch((cell_from >= 0) && (cell_from < Board::SIZE), cell_from, DEC);
        assert_val_mch(  (cell_to >= 0) && (cell_to < Board::SIZE),   (uint_mch)move, DEC);

        // pop piece from one place and write it to a new onee
        // with Piece::Shift parameter redefined as Shift::TRUE
        pce_mch* board = Board::get(board_index);

        switch (move)
        {
            case Move::STEP:
            case Move::DOUBLEPAWN:
            case Move::CAPTURE:
                board[cell_to] = (board[cell_from] & (pce_mch)Prop::TC) | (pce_mch)Shift::TRUE;
                Board::empty(board_index, cell_from);
                return;

            case Move::CASTLING:
                _make_move(board_index, cell_from, cell_to, Move::STEP);
                _make_move(board_index, get_castling((cell_from > cell_to) ? R : L), (cell_from + cell_to)/2, Move::STEP);
                return;

            case Move::EN_PASSANT:
                _make_move(board_index, cell_from, cell_to, Move::STEP);
                Board::empty(board_index, get_enpassant(cell_to));
                return;

            case Move::PROMOTION:
                #ifdef DEBUG
                    bool promtype_is_valid = false;
                    for (Type type : PROM_TYPES)
                        promtype_is_valid += (prom_type == type);

                    assert_val_mch(promtype_is_valid, (pce_mch)prom_type, DEC);
                #endif

                board[cell_to] = (pce_mch)prom_type | (pce_mch)get_color(board[cell_from]) | (pce_mch)Shift::TRUE;
                Board::empty(board_index, cell_from);
                return;

            default:
                assert_val_mch(0, (uint_mch)move, DEC);
                return;
        }
    }

    void _validate(coord_mch cell, coord_mch new_cell, Move move)
    {
        for (coord_mch sub_cell = 0; sub_cell < Board::SIZE; sub_cell++)
            Piece::calculate(Board::MINOR, sub_cell);

        if (Mask::king_is_hurt)
            Target::append(new_cell, Move::INVALID);
        else
        {
            Target::append(new_cell, move);
            Mask::append(cell, Mask::FRONTLINE);
        }
    }

    void _full_validate(coord_mch cell, coord_mch new_cell, Move move)
    {
        _make_move(Board::MINOR, cell, new_cell, move);
        _validate(cell, new_cell, move);
    }


    void calculate(Board::Index board_index, coord_mch cell)
    {
        pce_mch* board = Board::get(board_index);
        pce_mch  piece = board[cell];
        Color    color = get_color(piece);
        Type     type  = get_type(piece);

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


        coord_mch new_cell;
        pce_mch   new_piece;
        Type      new_type;
        Move      move;

        if (type == Type::PAWN)
        {
            int_mch sign = (color == Game::START_SIDE) ? 1 : -1;

            // #include <initializer_list> failed (perhaps, AVR compiler doesn't support this), so simply using an array
            Dir pawn_rays[] = { (Dir)(UR*sign), (Dir)(UL*sign) };
            for (Dir ray : pawn_rays)
            {
                new_cell = cell + ray;
                if (!Board::is_within(cell, new_cell)) continue;

                new_piece = board[new_cell];
                new_type  = get_type(new_piece);
                if (new_type != Type::EMPTY && get_color(new_piece) == color) continue;

                if (color == Game::current_side)
                {
                    Board::reset(Board::MINOR);

                    if (new_type != Type::EMPTY)
                        move = Move::CAPTURE;
                    else if (Game::last_cell == get_enpassant(new_cell) && Game::last_move == Move::DOUBLEPAWN)
                        move = Move::EN_PASSANT;
                    else continue;

                    _full_validate(cell, new_cell, move);
                    
                }
                else if (new_type == Type::KING)
                    king_hurt(board_index, cell);
            }

            if (color != Game::current_side)
                return;

            Dir ray = (Dir)(U*sign);
            
            new_cell = cell + ray;
            if (!(Board::is_within(cell, new_cell) && get_type(board[new_cell]) == Type::EMPTY))
                return;

            Board::reset(Board::MINOR);
            _full_validate(cell, new_cell, Move::STEP);

            new_cell += ray;
            if (!(Board::is_within(cell, new_cell) && get_type(board[new_cell]) == Type::EMPTY && get_shift(cell) == Shift::FALSE))
                return;

            Board::reset(Board::MINOR);
            _full_validate(cell, new_cell, Move::DOUBLEPAWN);

            return;
        }

        else if (get_prop(piece, Prop::TCS) == ((pce_mch)Type::KING | (pce_mch)Game::current_side | (pce_mch)Shift::FALSE))
        {
            Dir castling_rays[] = {R, L};

            for (Dir ray : castling_rays)
            {
                coord_mch rook_cell = get_castling(ray);
                if (get_prop(board[rook_cell], Prop::TCS) != ((pce_mch)Type::ROOK | (pce_mch)Game::current_side | (pce_mch)Shift::FALSE))
                    continue;

                new_cell = cell + ray;

                Board::reset(Board::MINOR);
                for (int_mch counter = 0; new_cell != rook_cell && get_type(board[new_cell]) == Type::EMPTY; counter++, new_cell += ray)
                {
                    if (counter < 2) Board::get(Board::MINOR)[new_cell] = piece;
                }
                if (new_cell == rook_cell)
                    _validate(cell, cell + ray*2, Move::CASTLING);
            }
        }

        // loop over all different directions suitable for the particular piece type
        for (int_mch ray_index = a; ray_index < b; ray_index++)
        {
            Dir ray = RAYS[ray_index];
            new_cell = cell;

            while (Board::is_within(new_cell, new_cell+ray))
            {
                new_cell += ray;
                new_piece = board[new_cell];
                new_type  = get_type(new_piece);

                bool new_is_empty = new_type == Type::EMPTY;
                if (!new_is_empty && get_color(new_piece) == color) break;

                if (color == Game::current_side)
                {
                    Board::reset(Board::MINOR);
                    _full_validate(cell, new_cell, (new_is_empty) ? Move::STEP : Move::CAPTURE);
                }
                // if the opponent manages to capture the King
                else if (new_type == Type::KING)
                    king_hurt(board_index, cell);

                // no reason to check more than one step in any direction
                // if the piece is of 'not-sliding' type (e.g. Knight)
                if (!(is_sliding && new_is_empty))
                    break;
            }
        }
    }
}