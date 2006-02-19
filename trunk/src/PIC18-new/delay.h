//**************************************************************
//*G9 Impulse SDK - NB_DELAY.H
//*Non blocking delay functionality
//*not very accurate, but checks input constantly
//*
//**************************************************************

#ifndef DELAY_H
#define DELAY_H

#include "common.h"

void delay_s(uint8 s);
void delay_ms(uint8 ms);
bool delayMs(uint8 ms);
bool delayS(uint8 s);

#endif
