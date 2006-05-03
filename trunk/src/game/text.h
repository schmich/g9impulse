#ifndef TEXT_H
#define TEXT_H

#include "common.h"
#include "point.h"

#define COLOR_WHITE 0
#define COLOR_BLUE 1

void drawText(const char* text, Point where, uint8 color);
void drawNumber(uint16 num, Point where, uint8 color);

uint16 textWidth(const char* text);

#endif // TEXT_H
