#include "sprite.h"

void drawSprite(Sprite* sprite)
{
    draw(sprite->image.address,
         sprite->image.width, sprite->image.height,
         sprite->position.x, sprite->position.y,
         sprite->alpha);
}
