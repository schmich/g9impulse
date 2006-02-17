#include <system.h>

#pragma CLOCK_FREQ 20000000						//required for accurate delay functionality

char queryNesController()
{
    char i;
    char input = 0;
    
	//assert strobe line high
    set_bit(portb, 6);
	delay_ms(1);
	clear_bit(portb, 6);
	delay_ms(1);
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
    
    return input;
}
  


void main()
{
	portb = 0;
	//according to datasheet, RX and TX must be 'inputs'
	trisb = 0b10000110;
	spbrg = 129;
	
	set_bit(txsta, BRGH);
	clear_bit(txsta, SYNC);
	set_bit(rcsta, SPEN);
	set_bit(txsta, TXEN);
	
	//poll nintendo controller every 10 ms or so
	//send results on serial port.

	char currentStatus, oldStatus, tmpCurrentStatus, tmpOldStatus;
	currentStatus = oldStatus = 0;
	
	char i;
	char mask = 0b00000001;
	char event;
	
	while (true)
	{
    	currentStatus = queryNesController();
    	if (currentStatus != oldStatus)
    	{
        	//then something changed
        	
        	tmpCurrentStatus = currentStatus;
        	tmpOldStatus = oldStatus;
        	
        	for (i = 0; i < 8; i++)
        	{
            	if ((tmpCurrentStatus & mask) != (tmpOldStatus & mask))
            	{
                	//rightmost bit changed
                	if (tmpCurrentStatus & mask) 
                	{
                    	//rightmost bit went down (pressed)
                    	event = (i & 0b01111111) | 0b10000000;
                    	txreg = event;
                    	delay_ms(1);
                    }
                    else
                    {
                    	//rightmost bit released
                    	event = (i & 0b01111111);
                    	txreg = event;
                    	delay_ms(1);
                    }
                }
                
                tmpCurrentStatus >>= 1;
                tmpOldStatus >>= 1;
            }
            
            oldStatus = currentStatus;
        }
    }		
}
	
		
	
	
