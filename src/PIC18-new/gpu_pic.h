//**************************************************************
//*G9 Impulse SDK - GPU.H
//*Provides essential GPU functionality for the PIC 16 and PIC18.
//*
//*
//**************************************************************

#ifndef GPU_PIC_H
#define GPU_PIC_H

#include "common.h"


typedef struct Point
{
    int32 x;
    int32 y;
} Point;


typedef struct Image
{
    uint32 address;
    uint8  width;
    uint8  height;
} Image;

/*typedef struct Bitmap
{
	uint32  	address;
	char		width;
	char	  	lines;
} Bitmap;
*/

typedef struct Entity
{
    Image* image;
    Point  position;
} Entity;

typedef struct Sprite
{
	Image 	        image;
	Point 	        position;
	char			visible;
	char  			alpha;

	//general purpose 8 bit parameters
	//wastes memory but makes code cleaner
	//would be better if this was C++ compiler :(
	char			a;
	char			b;
	char			x;
	char			y;
} Sprite;

//prototypes
void drawBackground(Image* background);
void drawSprite(const Sprite& sprite);

void drawImage(Image* image, Point where, bool isTransparent);
void drawEntity(Entity* entity);

void load_alphaOp( bool alphaOp);
void load_l_size(char size);
void load_s_lines(char lines);
void load_t_addr(unsigned long address);
void load_s_addr(unsigned long address);
void setDrawMode(bool doubleBuffer);
void flipBuffer(char & buf);
void setFieldColor(char color);
void gpuInit();
void draw();


#endif // GPU_PIC_H
