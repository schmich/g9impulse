#include "image.h"

Image makeImage(uint32 address, uint8 width, uint8 height)
{
    Image image;
    image.address = address;
    image.width   = width;
    image.height  = height;

    return image;
}

Image* createImage(uint32 address, uint8 width, uint8 height)
{
    Image* image = (Image*)alloc(sizeof(Image));
    image->address = address;
    image->width = width;
    image->height = height;

    return image;
}

void destroyImage(Image* image)
{
    free((void*)image);
}

void drawImage(Image* image, Point where, bool isTransparent)
{
    draw(image->address, image->width, image->height, where.x, where.y, isTransparent);
}

void drawBackground(Image* background)
{
    drawFullscreen(background->address);
}

Point makePoint(int16 x, int16 y)
{
    Point p;
    p.x = x;
    p.y = y;

    return p;
}
