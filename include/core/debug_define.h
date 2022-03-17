#pragma once

#if defined(DEBUG)
    #if defined(NDEBUG)
        #error DEBUG and NDEBUG defined simultaneously!
    #endif
#elif !defined(NDEBUG)
    #define NDEBUG
#endif