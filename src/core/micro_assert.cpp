#include "core/micro_assert.h"

void _init_assert()
{
    static bool _assert_initialized = false;
    if (_assert_initialized) return;

    #ifdef NDEBUG
    assert_forced_msg_mch(assert_mch is still active in RELEASE);
    #endif

    _assert_initialized = true;
}


void _ser_output()
{
    mserial_ps("FUNC\t"); mserial_pln(__PRETTY_FUNCTION__);
    mserial_ps("FILE\t"); mserial_pln(__FILE__);
    mserial_ps("LINE\t"); mserial_pln(__LINE__, DEC);

    _ser_abort();
}

void _ser_abort()
{
    mserial_flush();
    abort();
}