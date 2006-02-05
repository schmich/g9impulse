#define ROMSIZE  1	// 0 = 128kB
					// 0 = 256kB
					// 1 = 512kB
					// 2 = 1024kB

#include "I2C.c"


/*
void TXinit()
{
	set_bit(trisc, 7);
	set_bit(trisc, 6);

	//configure for 31.25Kbaud @ 50MHz
	spbrg = 19;	
	clear_bit(txsta, BRGH); 
	clear_bit(baudcon, BRG16);
	//asynchronous
	clear_bit(txsta, SYNC);
	set_bit(rcsta, SPEN);
	set_bit(txsta, TXEN);
	
	//don't receive midi for now
	clear_bit(rcsta, CREN);
	clear_bit(pie1, RCIE);
	
	//disable interrupts
	clear_bit(intcon, PEIE);
	clear_bit(intcon, GIE);
}

void RXinit()
{
	set_bit(trisc, 7);
	set_bit(trisc, 6);

	//configure for 31.25Kbaud @ 40MHz
	spbrg = 19;	
	clear_bit(txsta, BRGH);
	clear_bit(baudcon, BRG16);
	
	//asynchronous
	clear_bit(txsta, SYNC);
	set_bit(rcsta, SPEN);
	clear_bit(txsta, TXEN);

	//receive midi
	set_bit(rcsta, CREN);

	//disable interrupts
	clear_bit(pie1, RCIE);
	clear_bit(intcon, PEIE);
	clear_bit(intcon, GIE);
}

void midiTX(char midiword)
{
	txreg = midiword;
	delay_ms(1);
}

void playsound(char sound)
{
	//play percussive sound
	if (sound < 128)
	{		
		midiTX (0xC8);
		midiTX (127);
		
		midiTX (0x98);
		midiTX (60);
		midiTX (127);
		midiTX (0x88);
		midiTX (60);
		midiTX (127);
	}
}

char record_midi(unsigned int initaddr)
{
	// read RX port and wait for 1xxxxxxx
	// then read next two bytes sequentially
	// set this as current
	// repeat till end of song

	//this indicates we are in record mode
	set_bit(portb, 6);
	
	//temporary address (low and high)
	char addrlow    = 0;
	char addrhigh   = 0;
	//current MIDI word buffer
	char current    = 0;
	//current write address
	unsigned int address     = 0;
	//main timing counter
	unsigned int counter     = 0;
	char counthigh  = 0;
	char countlow   = 0;
	//MIDI count for debugging purposes
	unsigned int	midicount   = 0;
	
	//wait for the very first message, so hang until we get it
	while(!(pir1 & 0b00100000))
	{
		// wait for RXdata to be ready
	}
		
	current = rcreg;
	midicount ++;
	
	//write the current MIDI message first
	addr_eeprom(address);
	
	write_eeprom(current);
	address ++;
	
	//write dummy data into the next two words in order to maintain schema (the
	//first event has no preceeding event, so it's time is simply 0x00)
	write_eeprom(0x00);
	address ++;
	
	write_eeprom(0x00);
	address ++;

	//reset the counter
	counter = 0;
		
	while(true)
	{		
		//start the counter down here
		while(!(pir1 & 0b00100000))	// while I have no pending data
		{
			set_bit(portb, 4);
			
			if (counter == 0xFFFF)
			{
				set_bit(portb, 5);
				stop_eeprom();
				while (true)
				{
				}
			}
				
			//timer resolution is ~1ms
			delay_us(250);
			
			//count the time in ticks since I recorded the last MIDI message
			counter ++;
		
		}
		clear_bit(portb, 4);

			
		//when I have new data	
	
		current = rcreg;
		midicount ++;
		
		
		//light portb momentarily
		set_bit(portb, 7);
			
		//write the current MIDI message
		write_eeprom(current);
		address ++;
		
		//write the time in ticks since the last MIDI event, low bit first
		LOBYTE(countlow, counter);
		HIBYTE(counthigh, counter);
		
		write_eeprom(counthigh);
		address ++;
		
		write_eeprom(countlow);
		address ++;
		
		//reset the counter
		counter = 0;	
		
		//clear LED
		clear_bit(portb, 7);
	}
	return 1;
}// end record_midi

void play_midi(unsigned int address)
{	
	
	addr_eeprom(address);
	//this indicates we are in playback mode
	set_bit(portb, 7);
	
	//current MIDI word buffer
	char current    = 0;
	//current write address
	unsigned int address     = 0;
	//main timing counter
	unsigned int counter     = 0;
	char counthigh  = 0;
	char countlow   = 0;
	//MIDI count for debugging purposes
	unsigned int	midicount   = 0;
	
	while (true)
	{
		//light the other light when we're reading data
		set_bit(portb, 6);

		//first read out the midi message
		current = read_eeprom(address);
		address ++;
		
		counthigh = read_eeprom(address);
		address ++;
		
		countlow = read_eeprom(address);
		address ++;
		
		MAKESHORT(counter, countlow, counthigh);
		
		//light the other light when we're reading data
		clear_bit(portb, 6);
		
		for ( ; counter > 0; counter --)
		{
			//wait here until it is time to fire the event
		}
		
		midiTX (current);
	}
}
	
void main()
{
//	unsigned int counter = 0x0000;
	char done = 0;
	char i;
	
	i2c_init(10);
	portinit();
		
	delay_ms(100);
	
	while (true)
	{
	
		if ((portd & 0b00000001) && (done == 0)) // record
		{
			RXinit();
			done = record_midi(0);	
		}
		
		else if (done == 0)// playback
		{
			TXinit();
			play_midi(0);
		}
		else 
		{
			
		}
		
	} // end of while loop...


}


/*
#include <system.h>

#pragma CLOCK_FREQ 50000000						//required for accurate delay functionality

//initialize serial port for continuous receive
void midiinit()
{
	set_bit(trisc, 7);
	set_bit(trisc, 6);

	//configure for 31.25Kbaud
	spbrg = 24;	
	clear_bit(txsta, BRGH); 
	clear_bit(baudcon, BRG16);
	//asynchronous
	clear_bit(txsta, SYNC);
	set_bit(rcsta, SPEN);
	set_bit(txsta, TXEN);
	
	//don't receive midi for now
	clear_bit(rcsta, CREN);
	clear_bit(pie1, RCIE);
	
	//disable interrupts
	clear_bit(intcon, PEIE);
	clear_bit(intcon, GIE);
}

void portinit()
{
	//Configure port A
	adcon1 = 0x07;								//disable analog inputs
	trisa = 0x00;								//control output port
	//Configure port B							//data input port
	trisb = 0xFF;
	//Configure port C							//address input port
	trisc = 0xFF;
	//Configure port D							//MIDI and misc port
	trisd = 0xFF;								//port D is input from peripherals

	//Initialize port A
	porta = 0x00;
	//Initialize port B
	portb = 0x00;
	//Initialize port C
	portc = 0x00;
	//Initialize port D
	portd = 0x00;
}

void midiTX(char midiword)
{
	txreg = midiword;
	delay_ms(1);
}

void playsound(char sound)
{
	//play percussive sound
	if (sound < 128)
	{		
		midiTX (0xC8);
		midiTX (127);
		
		midiTX (0x98);
		midiTX (60);
		midiTX (127);
		midiTX (0x88);
		midiTX (60);
		midiTX (127);
	}
}
*/

