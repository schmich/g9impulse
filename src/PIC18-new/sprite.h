#ifndef SPRITE_H
#define SPRITE_H

#include "image.h"
#include "gpu_pic.h"

typedef struct Sprite
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
} Sprite;

void drawSprite(Sprite* sprite);

#endif // SPRITE_H
