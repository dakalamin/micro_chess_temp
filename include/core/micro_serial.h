#pragma once
#include <Arduino.h>

#ifdef SERIAL_SPEED
    #define mserial_begin()           { Serial.begin(SERIAL_SPEED); Serial.setTimeout(1); }
    #define mserial_end()             Serial.end()
    #define mserial_find(x)           Serial.find(x)
    #define mserial_flush()           Serial.flush()
    #define mserial_p(x, ...)         Serial.print(x, ##__VA_ARGS__)
    #define mserial_pln(x, ...)       Serial.println(x, ##__VA_ARGS__)
    #define mserial_ps(x, ...)        Serial.print(F(x), ##__VA_ARGS__)
    #define mserial_plns(x, ...)      Serial.println(F(x), ##__VA_ARGS__)
    #define mserial_read(...)         Serial.read(##__VA_ARGS__)
    #define mserial_read_bytes(x, y)  Serial.readBytes(x, y)
    #define mserial_write(...)        Serial.write(##__VA_ARGS__)
#else
    #define mserial_begin()           ((void)0)
    #define mserial_end()             ((void)0)
    #define mserial_find(...)         ((void)0)
    #define mserial_flush()           ((void)0)
    #define mserial_p(x, ...)         ((void)0)
    #define mserial_pln(x, ...)       ((void)0)
    #define mserial_ps(x, ...)        ((void)0)
    #define mserial_plns(x, ...)      ((void)0)
    #define mserial_read(...)         ((void)0)
    #define mserial_read_bytes(...)   ((void)0)
    #define mserial_write(...)        ((void)0)
#endif