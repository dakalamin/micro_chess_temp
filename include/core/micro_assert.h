#pragma once
#include "core/micro_serial.h"

#ifdef DEBUG
    #if defined(ASSERT_SERIAL) && !defined(SERIAL_SPEED)
        #pragma message("Warning: SERIAL_SPEED not defined -> ASSERT_SERIAL will be turned off!")
        #pragma message("                         (assert functionality is still active though)")
    #endif

    #ifdef ASSERT_SERIAL
        #define assert_mch(e) \
            _init_assert(); if (!(e)) { _ser_output(); }
        #define assert_msg_mch(e, m) \
            _init_assert(); if (!(e)) { mserial_ps(#m); _ser_output(); }
        #define assert_forced_msg_mch(m) \
            assert_msg_mch(false, m)
        #define assert_val_mch(e, v, ...) \
            _init_assert(); if (!(e)) { mserial_ps("VALUE\t"); mserial_pln(v, ##__VA_ARGS__); _ser_output(); }
        #define assert_forced_val_mch(v, ...) \
            assert_val_mch(false, v, ##__VA_ARGS__);
    #endif
#elif defined(ASSERT_SERIAL)
    #undef ASSERT_SERIAL
#endif

#ifndef ASSERT_SERIAL
    #define assert_mch(e)                  ((void)0)
    #define assert_msg_mch(e, m)           ((void)0)
    #define assert_forced_msg_mch(m)       ((void)0)
    #define assert_val_mch(e, v, ...)      ((void)0)
    #define assert_forced_val_mch(v, ...)  ((void)0)
#endif


void _init_assert();

void _ser_output();
void _ser_abort();