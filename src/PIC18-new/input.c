#include <system.h>
#include "input.h"


//initialize serial port for continuous receive
void serialInit()
{
	set_bit(trisc, 7);
	set_bit(trisc, 6);
	spbrg = 80;
	clear_bit(txsta, BRGH); 
	clear_bit(baudcon, BRG16);
	clear_bit(txsta, SYNC);
	set_bit(rcsta, SPEN);
	set_bit(txsta, TXEN);
	set_bit(rcsta, CREN);
	//disable interrupts
	clear_bit(pie1, RCIE);
	clear_bit(intcon, PEIE);
	clear_bit(intcon, GIE);
}
//non blocking input function
//returns the last known status of the input device
uint8 getInput()
{
	return rcreg;
}
