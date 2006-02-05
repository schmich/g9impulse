#include <system.h>
#include "gpu_pic.h"

//Loads the source address (registers 0, 1, and 2) into the GPU 
void load_s_addr(unsigned long address)
{	
	portc = 0x02; 								//Load least significant byte
	portb = (char) (0x000000FF & address);
	porta = 0x01;								//enable load
	porta = 0x00;								//load complete
	address >>= 8;
	portc = 0x01; 								//Load middle byte
	portb = (char) (0x000000FF & address);
	porta = 0x01;								//enable load
	porta = 0x00;								//load complete
	address >>= 8;
	portc = 0x00; 								//Load most significant byte
	portb = (char) (0x000000FF & address);
	porta = 0x01;								//enable load
	porta = 0x00;								//load complete
	return;
}

void load_t_addr(unsigned long address)
{	
	portc = 0x05; 								
	portb = (char) (0x000000FF & address);
	porta = 0x01;								
	porta = 0x00;								
	address >>= 8;
	portc = 0x04; 								
	portb = (char) (0x000000FF & address);
	porta = 0x01;								
	porta = 0x00;							
	address >>= 8;
	portc = 0x03; 								
	portb = (char) (0x000000FF & address);
	porta = 0x01;							
	porta = 0x00;
	return;
}

void load_s_lines(char lines)
{	
	portc = 0x06;
	portb = lines;
	porta = 0x01;
	porta = 0x00;
	return;
}

void load_l_size(char size)
{	
	portc = 0x07;
	portb = size;
	porta = 0x01;
	porta = 0x00;
	return;
}

void load_alphaOp( bool alphaOp)
{
	portc = 0x08;
	if (alphaOp)
		portb = 0x01;
	else
		portb = 0x00;
	porta = 0x01;
	porta = 0x00;
	return;
}

void set_drawmode( bool db)
{
	portc = 0x09;
	if (db)
		portb = 0x01;
	else
		portb = 0x00;
	porta = 0x01;
	porta = 0x00;
	return;
}

void set_fieldcolor( char color)
{
	portc = 0x0B;
	portb = color;
	porta = 0x01;
	porta = 0x00;
	return;
}


void flip_buffer(char & buf)
{
	portc = 0x0A;
	if (buf)
	{
		buf = 0;
		portb = 0x01;
	}
	else
	{
		buf = 1;
		portb = 0x00;
	}
	porta = 0x01;
	porta = 0x00;
	return;
}

void draw()
{
	porta = 00000010b;					//initialize draw
	porta = 00000000b;					//draw command recieved, so turn off draw bit
	while ( !(portd & 10000000b) )		//hang here till we recieve idle from GPU
	{
	}
}

//draw a fullscreen background
void drawtobackground (const Bitmap & source )
{
	load_t_addr(0x00000000);
	load_s_addr(source.address);
	load_s_lines(source.lines);
	load_l_size(source.width);
	load_alphaOp(false);
	draw();
}

long max(long p, long q)
{
	if (p > q)
		return p;
	else
		return q;
}

void drawsprite (const Sprite & sprite)
{
	//
	// we are assuming a screen of 160 addresses x 240 lines
	//

	if (!sprite.visible)
		return;

	unsigned long readAddress = sprite.image.address;
	unsigned long imageWidth = sprite.image.width;
	unsigned long imageLines = sprite.image.lines;

	long extentX = sprite.position.x + (long)sprite.image.width;
	if (extentX > 160)
	{
		//
		// right clipping
		//
		imageWidth -= (extentX - 160);
	}
	else if (sprite.position.x < 0)
	{
		//
		// left clipping
		//
		readAddress += -sprite.position.x;
		imageWidth -= -sprite.position.x;
	}

	long extentY = sprite.position.y + (long)sprite.image.lines;
	if (extentY > 240)
	{
		//
		// bottom clipping
		//
		imageLines -= (extentY - 240);
	}
	else if (sprite.position.y < 0)
	{
		//
		// top clipping
		//
		readAddress += (-sprite.position.y * sprite.image.width);
	}

	unsigned long destination_address = sprite.position.y * 160
									  + max(sprite.position.x, 0);

	load_t_addr(destination_address);
	load_s_addr(readAddress);
	load_s_lines(imageLines);
	load_l_size(imageWidth);

	if (sprite.alpha)
		load_alphaOp(true);
	else
		load_alphaOp(false);

	draw();
}

void gpuinit()
{
	//Configure port A
	adcon1 = 0x07;								//disable analog inputs
	trisa = 0x00;
	//Configure port B
	trisb = 0x00;
	//Configure port C
	trisc = 0x00;
	//Configure port D
	trisd = 11111111b;							//port D is input from peripherals

	//Initialize port A
	porta = 0x00;
	//Initialize port B
	portb = 0x00;
	//Initialize port C
	portc = 0x00;
	//Initialize port D
	portd = 0x00;
}
