#include "figure.h"

void drawFigure(Figure* figure)
{
    draw(figure->image.address,
         figure->image.width, figure->image.height,
         figure->position.x, figure->position.y,
         figure->alpha);
}
