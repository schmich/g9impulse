#include "image.h"

Image makeImage(uint32 address, uint8 width, uint8 height)
{
    Image image;
    image.address     = address;
    image.width       = width;
    image.height      = height;
    image.transparent = true;

    return image;
}

Image makeOpaqueImage(uint32 address, uint8 width, uint8 height)
{
    Image image;
    image.address     = address;
    image.width       = width;
    image.height      = height;
    image.transparent = false;

    return image;
}

Image* createImage(uint32 address, uint8 width, uint8 height)
{
    Image* image = new(Image);
    image->address     = address;
    image->width       = width;
    image->height      = height;
    image->transparent = true;

    return image;
}

Image* createOpaqueImage(uint32 address, uint8 width, uint8 height)
{
    Image* image = new(Image);
    image->address     = address;
    image->width       = width;
    image->height      = height;
    image->transparent = false;

    return image;
}

void destroyImage(Image* image)
{
    free(image);
}

void drawImage(Image* image, Point where, bool isTransparent)
{
    draw(image->address, image->width, image->height, where.x, where.y, isTransparent);
}

Point makePoint(int16 x, int16 y)
{
    Point p;
    p.x = x;
    p.y = y;

    return p;
}
