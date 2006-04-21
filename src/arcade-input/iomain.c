#include <stdlib.h>
#include <p18f452.h>
#include <delays.h>

//initialize serial port for continuous receive
void inputInit(void)
{
	TRISCbits.TRISC7 = 1;
	TRISCbits.TRISC6 = 0;
	SPBRG = 15;
	TXSTAbits.BRGH = 0;
	TXSTAbits.SYNC = 0;
	RCSTAbits.SPEN = 1;
	TXSTAbits.TXEN = 1;
	RCSTAbits.CREN = 0;
	PIE1bits.RCIE = 0;
	INTCONbits.PEIE = 0;
	INTCONbits.GIE = 0;
}


char queryArcadeController1()
{
	unsigned char input;
	input = PORTB;
	PORTB = 0x00;
	input = ~input;
    return input;
}

char queryArcadeController2()
{
	unsigned char input;
	input = PORTD;
	PORTD = 0x00;
	input = ~input;
    return input;
}
  
void main()
{
	unsigned char currentStatus1, oldStatus1, tmpCurrentStatus1, tmpOldStatus1;
	unsigned char currentStatus2, oldStatus2, tmpCurrentStatus2, tmpOldStatus2;
	unsigned char i;
	unsigned char mask = 0b00000001;
	unsigned char event;
	
	PORTB = 0x00;
	TRISB = 0xFF;
	PORTD = 0x00;
	TRISD = 0xFF;
	
	inputInit();
	//poll nintendo controller every 10 ms or so
	//send results on serial port.

	currentStatus = oldStatus = 0;
	
	while (1)
	{
    	currentStatus = queryArcadeController();
    	if (currentStatus != oldStatus)
    	{
        	//then something changed
        	
        	tmpCurrentStatus1 = currentStatus1;
        	tmpCurrentStatus2 = currentStatus2;
        	
        	tmpOldStatus1 = oldStatus1;
        	tmpOldStatus2 = oldStatus2;
        	

        	for (i = 0; i < 8; i++)
        	{
            	if ((tmpCurrentStatus & mask) != (tmpOldStatus & mask))
            	{
                	//rightmost bit changed
                	if (tmpCurrentStatus & mask) 
                	{
                    	//rightmost bit went down (pressed)
                    	event = (i & 0b01111111) | 0b10000000;
                    	TXREG = event;
                    	Delay1KTCYx(100);
                    }
                    else
                    {
                    	//rightmost bit released
                    	event = (i & 0b01111111);
                    	TXREG = event;
                    	Delay1KTCYx(100);
                    }
                }
                
                tmpCurrentStatus >>= 1;
                tmpOldStatus >>= 1;
            }
            
            oldStatus = currentStatus;
          
           
        }
    }		
}
	
		
	
	
