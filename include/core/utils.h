#include <stdint.h>

#define sgn(x) ((x) < 0) ? -1 : 1

using int_mch  = int8_t;   // general use signed 8-bit integer
using uint_mch = uint8_t;  // general use unsigned 8-bit integer

using pce_mch   = uint_mch;  // piece describing type (like individual pieces themselves or their props)
using mask_mch   = uint64_t;  // whole board describing type (like i/o masks)
using coord_mch = int_mch;   // board coordinate values


//const uint_mch DFLT_MASK = 0b10000000;