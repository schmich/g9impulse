//Non blocking delay functions

#include <system.h>
#include "input.h"
#include "nb_delay.h"

uint8 delayMs(uint16 ms)
{
	uint16 i;
	uint8 input;
	for (i = 0; i < ms; i ++)
	{
		delay_ms(1);
		input = getInput;
		if (input) 
			return input;
	}
}
				
uint8 delays(uint8 s)
{
	uint16 ms = (s * 1000);
	uint16 i;
	uint8 input;
	
	for (i = 0; i < ms; i ++)
	{
		delay_ms(1);
		input = getInput;
		if (input) 
			return input;
	}
}
			
