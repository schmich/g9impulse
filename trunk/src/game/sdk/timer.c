#include <timers.h>
#include "timer.h"

void initTimer(void)
{
    uint8 config = TIMER_INT_OFF &
                   T0_16BIT &
                   T0_SOURCE_INT &
                   T0_EDGE_RISE &
                   T0_PS_1_256;

    OpenTimer0(config);
}

void resetTimer(void)
{
    WriteTimer0(0);
}

uint16 timeElapsed(void)
{
    return ReadTimer0();
}
