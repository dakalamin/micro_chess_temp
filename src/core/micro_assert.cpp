#include "core/micro_assert.h"

void __ser_output(const char *__func, const char *__file, int __lineno)
{
    Serial.println();
    Serial.print(F("FUNC\t")); Serial.println(__func);
    Serial.print(F("FILE\t")); Serial.println(__file);
    Serial.print(F("LINE\t")); Serial.println(__lineno, DEC);
}

void __ser_abort()
{
    Serial.flush();
    abort();
}