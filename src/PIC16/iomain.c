#include <system.h>

#pragma CLOCK_FREQ 4000000						//required for accurate delay functionality

void main()
{
	portb = 0;
	//according to datasheet, RX and TX must be 'inputs'
	trisb = 0b10000110;
	spbrg = 25;
	
	set_bit(txsta, BRGH);
	clear_bit(txsta, SYNC);
	set_bit(rcsta, SPEN);
	set_bit(txsta, TXEN);
	
	//poll nintendo controller every 10 ms or so
	//send results on serial port.
	char i, input;
	while (true)
	{
		//assert strobe line high
		set_bit(portb, 6);
		delay_ms(1);
		clear_bit(portb, 6);
		delay_ms(1);
		//read 8 bits here (read performs inversion)
		for (i = 0; i < 8; i++)
		{
			input <<= 1;
			if (portb & 0b10000000)
			{
				clear_bit(input, 0);	
			}
			else 
			{
				set_bit(input, 0);	
			}
			
			//toggle clock
			delay_ms(1);
			set_bit(portb, 5);
			delay_ms(1);
			clear_bit(portb, 5);
			delay_ms(1);
		}
		txreg = input;
		delay_ms(10);
	}
		
}
	
		
	
	
