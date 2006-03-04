#ifndef FIGURE_H
#define FIGURE_H

#include "image.h"
#include "gpu.h"

typedef struct Figure
{
    Image image;
    Point position;
    char  visible;
    char  alpha;

    //general purpose 8 bit parameters
    //wastes memory but makes code cleaner
    //would be better if this was C++ compiler :(
    char  a;
    char  b;
    char  x;
    char  y;
} Figure;

void drawFigure(Figure* figure);

#endif // FIGURE_H
