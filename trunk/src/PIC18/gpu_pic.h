//**************************************************************
//*G9 Impulse SDK - GPU.H
//*Provides essential GPU functionality for the PIC 16 and PIC18.
//*
//*
//**************************************************************

#ifndef _GPU_OPS_H
#define _GPU_OPS_H

//types
struct Point
{
	long x;
	long y;
};

struct Bitmap
{
	unsigned long 	address;
	char		width;
	char	  	lines;
};

struct Sprite
{
	struct Bitmap 	image;
	struct Point 	position;
	char			visible;
	char  			alpha;

	//general purpose 8 bit parameters
	//wastes memory but makes code cleaner
	//would be better if this was C++ compiler :(
	char			a;
	char			b;
	char			x;
	char			y;

	/*
	union a
	{
		char			frame;	// animation frame
		char			roll;	// player roll
		char			health;	// enemy's health
	};

	union b
	{
		char			animationTimer;
		char			yDestination;
		char			heat;	// player's gun's heat
	};
	*/
};

typedef struct Bitmap Bitmap;
typedef struct Point Point;
typedef struct Sprite Sprite;

//prototypes
void drawtobackground ( Bitmap source );
void drawsprite (Sprite sprite);
void load_alphaOp( bool alphaOp);
void load_l_size(char size);
void load_s_lines(char lines);
void load_t_addr(unsigned long address);
void load_s_addr(unsigned long address);
void set_drawmode (bool db);
void flip_buffer (char & buf);
void set_fieldcolor (char color);
void gpuinit();
void draw();

#endif
