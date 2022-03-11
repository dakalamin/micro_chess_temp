#include "mask.h"
#include "coords.h"

namespace Mask
{
    mask_mch _buffer[_buffer_size];
    bool _king_is_hurt;

    void print(Index mask_index)
    {
        Coords::print_foreach(
        [mask_index](coord_mch cell) { mserial_p(check(cell, mask_index), BIN); },
        1, true );
    }
}