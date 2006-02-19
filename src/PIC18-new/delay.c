//Non blocking delay functions

#include <delays.h>
#include "input.h"
#include "delay.h"

void delay_ms(uint8 ms)
{
	uint8 i;
	for (i = 0; i < ms; i++)
	{
		Delay1KTCYx(12);
		Delay100TCYx(5);
	}
}

void delay_s(uint8 s)
{
	uint16 ms = s * 1000;
	uint16 i;
	for (i = 0; i < ms; i++)
	{
		Delay1KTCYx(12);
		Delay100TCYx(5);
	}
}

bool delayMs(uint8 ms)
{
    uint8 i;
    uint8 input;
	Input* event;

    for (i = 0; i < ms; i ++)
    {
        delay_ms(1);
        event = getInputEvent();
        if (event->anyInput) 
            return true;
    }

    //if no input, return false
    return false;
}
                
uint8 delayS(uint8 s)
{
    uint16 ms = (s * 1000);
    uint16 i;
    uint8 input;
    Input* event;
    
    for (i = 0; i < ms; i ++)
    {
        delay_ms(1);
        event = getInputEvent();
        if (event->anyInput) 
            return true;
    }

    //if no input, return false
    return false;
}
