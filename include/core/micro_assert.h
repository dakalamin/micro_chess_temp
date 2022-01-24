#pragma once
#include <Arduino.h>
#include "core/utils.h"

#ifdef DEBUG
    #if defined(ASSERT_SERIAL) && defined(SERIAL_SPEED)
        #define assert_mch(e) \
            if (!(e)) { __ser_output(__func__, __FILE__, __LINE__); __ser_abort(); }
        #define assert_msg_mch(e, m) \
            if (!(e)) { __ser_output(__func__, __FILE__, __LINE__); Serial.print(F(#m)); __ser_abort(); };
        #define assert_val_mch(e, v, ...) \
            if (!(e)) { __ser_output(__func__, __FILE__, __LINE__); Serial.print(F("VALUE\t")); Serial.println(v, ##__VA_ARGS__); __ser_abort(); };
    #else
        #define assert_mch(e)             ((e) ? (void)0 : __ser_abort())
        #define assert_msg_mch(e, m)      ((e) ? (void)0 : __ser_abort())
        #define assert_val_mch(e, v, ...) ((e) ? (void)0 : __ser_abort())
    #endif
#else
    #define assert_mch(e)             ((void)0)
    #define assert_msg_mch(e, m)      ((void)0)
    #define assert_val_mch(e, v, ...) ((void)0)
#endif

void __ser_output(const char *, const char *, int __lineno);
void __ser_abort();