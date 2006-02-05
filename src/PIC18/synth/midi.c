#include <system.h>
#pragma CLOCK_FREQ 1000000

void portinit()
{
	//Configure port A
	adcon1 = 0x07;								//disable analog inputs
	trisa = 0xFF;								//control output port
	//Configure port B							//data input port
	trisb = 0xFF;
	//Configure port C							//address input port
	trisc = 0xFF;
	//Configure port D
	trisd = 0x00;								//portd goes out to the mp3 player
	//Initialize port A
	porta = 0x00;
	//Initialize port B
	portb = 0x00;
	//Initialize port C
	portc = 0x00;
	//Initialize port D
	portd = 0b11111111;
}
	
void main()
{
	portd = 0xFF;
	char i = 0;
	char currenttrack = 0;
	char playtrack = 0;
	portinit();
	
	delay_s(2);
	
	//on reset play and stop the first song
	clear_bit(portd, 0);
	delay_ms(20);
	set_bit(portd, 0);
	
	//wait while it's playing the intro song
	delay_s(6);
	//stop the intro song
	clear_bit(portd, 1);
	delay_ms(100);
	set_bit(portd, 1);
	
	while (true)
	{
	//when load is enabled
		if (portc & 0b10000000)
		{
			//play sound effect (register 15)
			if ( (portc  & 0x0F) == 0x0F)
			{	
				currenttrack = portb;
				if (currenttrack == 255)
				{
					//stop the current track
					clear_bit(portd, 1);
					delay_ms(100);
					set_bit(portd, 1);
					delay_ms(50);
				}
				else 
				{
					while (currenttrack != playtrack)
					{
						if (currenttrack > playtrack)
						{
							playtrack ++;
							//toggle next track button
							clear_bit(portd, 3);
							delay_ms(50);
							set_bit(portd, 3);
							delay_ms(50);
						}
						else 
						{
							playtrack --;
							clear_bit(portd, 2);
							delay_ms(50);
							set_bit(portd, 2);
							delay_ms(50);
						}
				}
				clear_bit(portd, 0);
				delay_ms(20);
				set_bit(portd, 0);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
				}
			}
		}
	}
}
