#include <system.h>
#include "gpu_pic.h"

#pragma CLOCK_FREQ 50000000						//required for accurate delay functionality

void bootup(void)
{
	char i = 0;
	char j = 0;

	Bitmap black;
		black.address = 0x0001C200;
		black.lines = 0xF0;
		black.width = 0xA0;

	Sprite frame;
		frame.image.address = 0x00376800;
		frame.image.lines = 160;
		frame.image.width = 79;
		frame.position.x = 40;
		frame.position.y = 40;
		frame.alpha = 0;
	
	drawtobackground(black);
		
	delay_ms(200);

	//play boot movie
	for (i = 0; i <  21; i++)
	{
		//show 'left' frame
		drawsprite (frame);
		if (i == 0)
		{
			delay_ms(250);
			delay_ms(250);
		}
		
		//show 'right' frame
		if (i > 18)
			delay_ms(100);
		else
			delay_ms(60);
		frame.image.address += 80;
		drawsprite (frame);
		if (i > 18)
			delay_ms(100);
		else
			delay_ms(60);
		//skip down to next two frames
		frame.image.address += 25520;	
	}
	return;
}


void main()
{
	//various variables related only to main
	char i = 0;
		
	//initalize GPU ports
	gpuinit();
	while (true)
	{
		bootup();						//draw bootup logo
	}
}
