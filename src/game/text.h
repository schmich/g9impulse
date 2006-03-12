#ifndef TEXT_H
#define TEXT_H

#include "common.h"
#include "point.h"

void drawText(const rom char* text, Point where);
void drawNumber(uint16 num, Point where);

#endif // TEXT_H
