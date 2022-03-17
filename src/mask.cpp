#include "mask.h"

namespace Mask
{
    int_mch _buffer[_buffer_index_size][_buffer_elems_size];


    bool check(Index mask_index)
    {
        for (int_mch i = 0; i < _buffer_elems_size; i++)
            if (!get(mask_index)[i]) return false;

        return true;
    }

    void reset(Index mask_index)
    {
        for (int_mch i = 0; i < _buffer_elems_size; i++)
            get(mask_index)[i] = 0;
    }
}