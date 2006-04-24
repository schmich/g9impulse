#ifndef TEXT_H
#define TEXT_H

#include "common.h"
#include "point.h"

void drawText(const char* text, Point where);
void drawNumber(uint16 num, Point where);

uint16 textWidth(const char* text);

#endif // TEXT_H
