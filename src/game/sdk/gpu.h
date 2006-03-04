//**************************************************************
//*G9 Impulse SDK - GPU.H
//*Provides essential GPU functionality for the PIC 16 and PIC18.
//*
//*
//**************************************************************

#ifndef GPU_H
#define GPU_H

#include "common.h"

void gpuInit(void);

void draw(uint32 address, uint16 width, uint16 height, int16 x, int16 y, bool isTransparent);
void drawFullscreen(uint32 address);

void load_alphaOp(bool alphaOp);
void load_l_size(uint8 size);
void load_s_lines(uint8 lines);
void load_t_addr(unsigned long address);
void load_s_addr(unsigned long address);

void setFieldColor(uint8 color);
void setDoubleBuffer(bool doubleBuffer);
void flipBuffer(uint8* buf);

#endif // GPU_H
